#include "localization/localization_node.hpp"

#include <chrono>
#include <cmath>
#include <functional>
#include <stdexcept>

#include "localization/robust_stats.hpp"

namespace localization
{

namespace
{

// 从偏航角生成绕 z 轴的四元数。
geometry_msgs::msg::Quaternion yawToQuaternion(double psi)
{
    geometry_msgs::msg::Quaternion quat;
    quat.x = 0.0;
    quat.y = 0.0;
    quat.z = std::sin(psi / 2.0);
    quat.w = std::cos(psi / 2.0);
    return quat;
}

// 状态码 → 可读字符串（仅用于日志）。
const char * statusName(SolveStatus status)
{
    switch (status)
    {
        case SolveStatus::kOk: return "OK";
        case SolveStatus::kDeviated: return "DEVIATED";
        case SolveStatus::kLowObservability: return "LOW_OBSERVABILITY";
        case SolveStatus::kNotEnoughSamples: return "NOT_ENOUGH_SAMPLES";
        case SolveStatus::kSolveFailed: return "SOLVE_FAILED";
    }
    return "UNKNOWN";
}

}  // namespace

LocalizationNode::LocalizationNode(const rclcpp::NodeOptions & options)
: Node("localization_node", options),
  adapter_(declare_parameter<std::vector<int64_t>>("channel_indices", {0, 1, 2})),
  relocalizer_(SolverParams{})
{
    // ---- 数据源 ----
    const std::string distance_topic = declare_parameter<std::string>(
        "distance_topic", "/get_serial_msg/distances");
    frame_id_ = declare_parameter<std::string>("frame_id", "map");

    // ---- 采样 ----
    sample_duration_ = declare_parameter<double>("sample_duration", 3.0);
    if (sample_duration_ <= 0.0)
    {
        RCLCPP_ERROR(get_logger(), "sample_duration must be positive, got %.3f",
                     sample_duration_);
        throw std::invalid_argument("sample_duration must be positive");
    }
    min_samples_ = declare_parameter<int64_t>("min_samples", 50);
    if (min_samples_ < 1)
    {
        RCLCPP_ERROR(get_logger(), "min_samples must be at least 1, got %ld",
                     static_cast<long>(min_samples_));
        throw std::invalid_argument("min_samples must be at least 1");
    }

    // ---- 先验 q0 / P0 ----
    const double q0_x = declare_parameter<double>("q0_x", 0.0);
    const double q0_y = declare_parameter<double>("q0_y", 0.0);
    const double q0_psi = declare_parameter<double>("q0_psi", 0.0);
    const double sigma_x0 = declare_parameter<double>("sigma_x0", 0.5);
    const double sigma_y0 = declare_parameter<double>("sigma_y0", 0.5);
    const double sigma_psi0 = declare_parameter<double>("sigma_psi0", 0.35);
    if (sigma_x0 <= 0.0 || sigma_y0 <= 0.0 || sigma_psi0 <= 0.0)
    {
        RCLCPP_ERROR(get_logger(), "sigma_x0/sigma_y0/sigma_psi0 must be positive");
        throw std::invalid_argument("prior sigmas must be positive");
    }
    q0_ = Eigen::Vector3d(q0_x, q0_y, q0_psi);
    P0_ = Eigen::Vector3d(sigma_x0 * sigma_x0, sigma_y0 * sigma_y0,
                          sigma_psi0 * sigma_psi0).asDiagonal();

    // ---- 求解参数 ----
    solver_params_.denom_min_abs = declare_parameter<double>("denom_min_abs", 1.0e-6);
    solver_params_.gn_max_iter = static_cast<uint32_t>(
        declare_parameter<int64_t>("gn_max_iter", 20));
    solver_params_.gn_step_tol = declare_parameter<double>("gn_step_tol", 1.0e-8);
    solver_params_.gn_diff_eps = declare_parameter<double>("gn_diff_eps", 1.0e-6);
    solver_params_.yaw_max = declare_parameter<double>("yaw_max", 1.0);
    solver_params_.cond_warn_threshold =
        declare_parameter<double>("cond_warn_threshold", 1000.0);
    solver_params_.mahalanobis_threshold =
        declare_parameter<double>("mahalanobis_threshold", 11.34);
    relocalizer_.setParams(solver_params_);

    // ---- 传感器标定 ----
    sensors_[0] = loadSensorConfig("dt1");
    sensors_[1] = loadSensorConfig("dt2");
    sensors_[2] = loadSensorConfig("dt3");

    // ---- 通信接口 ----
    sub_ = create_subscription<get_serial_msg::msg::Dt35Distances>(
        distance_topic, rclcpp::QoS(10),
        std::bind(&LocalizationNode::onDistances, this, std::placeholders::_1));
    result_pub_ = create_publisher<msg::RelocalizationResult>("~/result", rclcpp::QoS(10));
    pose_pub_ = create_publisher<geometry_msgs::msg::PoseWithCovarianceStamped>(
        "~/pose", rclcpp::QoS(10));
    resample_srv_ = create_service<std_srvs::srv::Trigger>(
        "~/resample",
        std::bind(&LocalizationNode::onResample, this,
                  std::placeholders::_1, std::placeholders::_2));

    sample_window_timer_ = create_wall_timer(
        std::chrono::duration<double>(sample_duration_),
        std::bind(&LocalizationNode::finishSampling, this));
    // 先取消定时器，由 startSampling() 统一开启。
    sample_window_timer_->cancel();

    RCLCPP_INFO(get_logger(),
                "Localization node ready: 3 channels from '%s', sample %.1f s, "
                "min samples %ld, q0=[%.3f, %.3f, %.3f]",
                distance_topic.c_str(), sample_duration_,
                static_cast<long>(min_samples_), q0_.x(), q0_.y(), q0_.z());

    startSampling();
}

SensorConfig LocalizationNode::loadSensorConfig(const std::string & prefix)
{
    SensorConfig sc;
    const auto s = declare_parameter<std::vector<double>>(prefix + ".s", {0.0, 0.0, 0.0});
    const auto u = declare_parameter<std::vector<double>>(prefix + ".u", {1.0, 0.0, 0.0});
    const auto n = declare_parameter<std::vector<double>>(prefix + ".n", {1.0, 0.0, 0.0});
    if (s.size() != 3U || u.size() != 3U || n.size() != 3U)
    {
        RCLCPP_ERROR(get_logger(), "%s: s/u/n must each be a 3-element vector",
                     prefix.c_str());
        throw std::invalid_argument(prefix + ": s/u/n must be 3-element vectors");
    }

    const Eigen::Vector3d u_raw(u[0], u[1], u[2]);
    const Eigen::Vector3d n_raw(n[0], n[1], n[2]);
    if (u_raw.norm() < 1.0e-12 || n_raw.norm() < 1.0e-12)
    {
        RCLCPP_ERROR(get_logger(), "%s: u and n must be non-zero vectors",
                     prefix.c_str());
        throw std::invalid_argument(prefix + ": u/n must be non-zero");
    }
    // 需求约定为单位向量，这里统一归一化，避免配置手写误差。
    sc.s = Eigen::Vector3d(s[0], s[1], s[2]);
    sc.u = u_raw.normalized();
    sc.n = n_raw.normalized();

    sc.c = declare_parameter<double>(prefix + ".c", 0.0);
    sc.b = declare_parameter<double>(prefix + ".b", 0.0);
    sc.k = declare_parameter<double>(prefix + ".k", 1.0);
    sc.sigma_d = declare_parameter<double>(prefix + ".sigma_d", 0.01);
    sc.sigma_sys = declare_parameter<double>(prefix + ".sigma_sys", 0.02);
    if (sc.k <= 0.0 || sc.sigma_d < 0.0 || sc.sigma_sys < 0.0 ||
        sc.sigma_d + sc.sigma_sys <= 0.0)
    {
        RCLCPP_ERROR(get_logger(),
                     "%s: k must be positive and sigma_d/sigma_sys non-negative "
                     "(not both zero)", prefix.c_str());
        throw std::invalid_argument(prefix + ": invalid k or sigma values");
    }
    return sc;
}

void LocalizationNode::startSampling()
{
    for (auto & samples : samples_)
    {
        samples.clear();
    }
    frame_count_ = 0U;
    sampling_ = true;
    sample_window_timer_->reset();
    RCLCPP_INFO(get_logger(), "Sampling started (%.1f s window)...", sample_duration_);
}

void LocalizationNode::onDistances(const get_serial_msg::msg::Dt35Distances::SharedPtr msg)
{
    if (!sampling_)
    {
        return;
    }
    const Reading reading = adapter_.Convert(*msg);
    frame_count_++;
    for (size_t j = 0U; j < 3U; ++j)
    {
        if (reading.valid[j])
        {
            samples_[j].push_back(reading.distance_m[j]);
        }
    }
}

void LocalizationNode::finishSampling()
{
    if (!sampling_)
    {
        return;
    }
    sampling_ = false;
    sample_window_timer_->cancel();

    // 每通道稳健统计：中位数 + MAD 剔异常 → 均值。
    Eigen::Vector3d z_mean = Eigen::Vector3d::Zero();
    std::array<size_t, 3> sample_counts{{0U, 0U, 0U}};
    for (size_t j = 0U; j < 3U; ++j)
    {
        z_mean[static_cast<Eigen::Index>(j)] = RobustMean(samples_[j], &sample_counts[j]);
    }

    RCLCPP_INFO(get_logger(),
                "Sampling done: %u frames, inliers [%zu, %zu, %zu], "
                "z_mean [%.3f, %.3f, %.3f] m",
                frame_count_, sample_counts[0], sample_counts[1], sample_counts[2],
                z_mean.x(), z_mean.y(), z_mean.z());

    for (size_t j = 0U; j < 3U; ++j)
    {
        if (sample_counts[j] < static_cast<size_t>(min_samples_))
        {
            RCLCPP_WARN(get_logger(),
                        "Channel %zu has only %zu inliers (< %ld), not enough "
                        "samples to solve", j + 1U, sample_counts[j],
                        static_cast<long>(min_samples_));
            publishNotEnoughSamples(z_mean, sample_counts);
            return;
        }
    }

    const SolveResult result = relocalizer_.Solve(sensors_, z_mean, q0_, P0_);
    publishResult(result, z_mean, sample_counts);
}

void LocalizationNode::publishResult(const SolveResult & result,
                                     const Eigen::Vector3d & z_mean,
                                     const std::array<size_t, 3> & sample_counts)
{
    msg::RelocalizationResult out;
    out.header.stamp = now();
    out.header.frame_id = frame_id_;
    out.status = static_cast<uint8_t>(result.status);
    out.x = result.q_hat.x();
    out.y = result.q_hat.y();
    out.psi = result.q_hat.z();
    // 3x3 协方差按行主序展开，顺序 x / y / psi。
    out.covariance = {
        result.P(0, 0), result.P(0, 1), result.P(0, 2),
        result.P(1, 0), result.P(1, 1), result.P(1, 2),
        result.P(2, 0), result.P(2, 1), result.P(2, 2)};
    out.delta = {result.delta.x(), result.delta.y(), result.delta.z()};
    out.mahalanobis_distance = result.mahalanobis;
    out.condition_number = result.condition_number;
    out.sample_counts = {
        static_cast<uint32_t>(sample_counts[0]),
        static_cast<uint32_t>(sample_counts[1]),
        static_cast<uint32_t>(sample_counts[2])};
    out.frame_count = frame_count_;
    out.z_mean = {z_mean.x(), z_mean.y(), z_mean.z()};
    out.iterations = result.iterations;
    result_pub_->publish(out);

    // 同时发布标准位姿消息，方便 rviz 等工具直接可视化。
    geometry_msgs::msg::PoseWithCovarianceStamped pose;
    pose.header = out.header;
    pose.pose.pose.position.x = out.x;
    pose.pose.pose.position.y = out.y;
    pose.pose.pose.position.z = 0.0;
    pose.pose.pose.orientation = yawToQuaternion(out.psi);
    // 6x6 协方差（行主序），只填 x / y / yaw 三个分量。
    auto & covariance = pose.pose.covariance;
    covariance.fill(0.0);
    covariance[0] = result.P(0, 0);
    covariance[1] = result.P(0, 1);
    covariance[5] = result.P(0, 2);
    covariance[6] = result.P(1, 0);
    covariance[7] = result.P(1, 1);
    covariance[11] = result.P(1, 2);
    covariance[30] = result.P(2, 0);
    covariance[31] = result.P(2, 1);
    covariance[35] = result.P(2, 2);
    pose_pub_->publish(pose);

    RCLCPP_INFO(get_logger(),
                "Result: %s, q_hat=[%.3f, %.3f, %.3f], delta=[%.3f, %.3f, %.3f], "
                "mahalanobis=%.3f, cond=%.3g, iterations=%u",
                statusName(result.status),
                result.q_hat.x(), result.q_hat.y(), result.q_hat.z(),
                result.delta.x(), result.delta.y(), result.delta.z(),
                result.mahalanobis, result.condition_number, result.iterations);
}

void LocalizationNode::publishNotEnoughSamples(
    const Eigen::Vector3d & z_mean,
    const std::array<size_t, 3> & sample_counts)
{
    // 样本不足：位姿退化为先验 q0，协方差退化为 P0。
    SolveResult result;
    result.status = SolveStatus::kNotEnoughSamples;
    result.q_hat = q0_;
    result.P = P0_;
    result.delta = Eigen::Vector3d::Zero();
    publishResult(result, z_mean, sample_counts);
}

void LocalizationNode::onResample(
    const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
    std::shared_ptr<std_srvs::srv::Trigger::Response> response)
{
    (void)request;
    startSampling();
    response->success = true;
    response->message = "Resampling started";
}

}  // namespace localization
