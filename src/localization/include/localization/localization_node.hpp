// 三测距仪 + 双墙平面定位节点。
//
// 工作流程（状态机）：
//   1. 启动后进入采样态，静止采样 sample_duration 秒（期间车体必须保持不动）；
//   2. 窗口结束后，每通道中位数 + MAD 剔异常求均值 z_mean；
//      任一通道样本数 < min_samples → 状态 NOT_ENOUGH_SAMPLES，不求解；
//   3. 带先验 Gauss-Newton 求解 q_hat 与协方差 P；
//   4. 偏离检测（q0 处马氏距离）与可观测性检查（信息矩阵 A 的条件数）；
//   5. 发布结果；通过 ~/resample 服务可随时重新采样求解。
//
// 单位约定：毫米 → 米的换算只在 DistanceAdapter 里做一次，之后全链路用米。

#ifndef LOCALIZATION__LOCALIZATION_NODE_HPP_
#define LOCALIZATION__LOCALIZATION_NODE_HPP_

#include <array>
#include <cstdint>
#include <string>
#include <vector>

#include "rclcpp/rclcpp.hpp"
#include "std_srvs/srv/trigger.hpp"
#include "geometry_msgs/msg/pose_with_covariance_stamped.hpp"
#include "get_serial_msg/msg/dt35_distances.hpp"

#include "localization/distance_adapter.hpp"
#include "localization/relocalizer.hpp"
#include "localization/msg/relocalization_result.hpp"

namespace localization
{

class LocalizationNode : public rclcpp::Node
{
public:
    explicit LocalizationNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

private:
    // 数据回调：适配层换算后，采样期间按通道累积样本。
    void onDistances(const get_serial_msg::msg::Dt35Distances::SharedPtr msg);
    // 采样窗口结束：统计 → 求解 → 发布。
    void finishSampling();
    // （重新）开始一次采样。
    void startSampling();
    // ~/resample 服务回调：触发重新采样求解。
    void onResample(const std::shared_ptr<std_srvs::srv::Trigger::Request> request,
                    std::shared_ptr<std_srvs::srv::Trigger::Response> response);
    // 发布结果消息与标准位姿消息。
    void publishResult(const SolveResult & result,
                       const Eigen::Vector3d & z_mean,
                       const std::array<size_t, 3> & sample_counts);
    // 样本不足时发布退化结果（q_hat = q0，P = P0）。
    void publishNotEnoughSamples(const Eigen::Vector3d & z_mean,
                                 const std::array<size_t, 3> & sample_counts);
    // 从参数加载单个传感器标定（dt1 / dt2 / dt3）。
    SensorConfig loadSensorConfig(const std::string & prefix);

    rclcpp::Subscription<get_serial_msg::msg::Dt35Distances>::SharedPtr sub_;
    rclcpp::Publisher<msg::RelocalizationResult>::SharedPtr result_pub_;
    rclcpp::Publisher<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr pose_pub_;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr resample_srv_;
    rclcpp::TimerBase::SharedPtr sample_window_timer_;

    DistanceAdapter adapter_;
    Relocalizer relocalizer_;

    std::array<SensorConfig, 3> sensors_;

    // 先验 q0 / P0。
    Eigen::Vector3d q0_{Eigen::Vector3d::Zero()};
    Eigen::Matrix3d P0_{Eigen::Matrix3d::Zero()};

    // 采样状态（单线程执行器下由订阅回调与采样定时器访问，天然安全）。
    bool sampling_ = false;
    std::array<std::vector<double>, 3> samples_;
    uint32_t frame_count_ = 0U;

    // 参数快照（启动时读取一次）。
    std::string frame_id_;
    double sample_duration_ = 3.0;
    int64_t min_samples_ = 50;
    SolverParams solver_params_;
};

}  // namespace localization

#endif  // LOCALIZATION__LOCALIZATION_NODE_HPP_
