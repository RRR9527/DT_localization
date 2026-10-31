#include "localization/localization_node.hpp"

#include <cmath>
#include <functional>
#include <stdexcept>

namespace localization
{

LocalizationNode::LocalizationNode(const rclcpp::NodeOptions & options)
: Node("localization_node", options)
{
    channel_1_ = declare_parameter<int>("channel_1", 0);
    channel_2_ = declare_parameter<int>("channel_2", 2);
    channel_3_ = declare_parameter<int>("channel_3", 3);
    l_ = declare_parameter<double>("l", 1.0);
    len_x = declare_parameter<double>("len_x", 0.5);
    len_y = declare_parameter<double>("len_y", 0.5);
    calibration_duration = 
        declare_parameter<double>("calibration_duration", 2.0);

    bias_ch_1 = declare_parameter<double>("bias_ch_1", 0.0);
    bias_ch_2 = declare_parameter<double>("bias_ch_2", 0.0);
    bias_ch_3 = declare_parameter<double>("bias_ch_3", 0.0);
    bias_d3_alpha = declare_parameter<double>("bias_d3_alpha", 0.0);
    bias_yaw = declare_parameter<double>("bias_yaw", 0.0);
    bias_Xaxis = declare_parameter<double>("bias_Xaxis", 0.0);

    // 通道下标必须是 [0, 3] 内互不相同的三个数。
    if 
    (
        channel_1_ < 0 || channel_1_ > 3 ||
        channel_2_ < 0 || channel_2_ > 3 ||
        channel_3_ < 0 || channel_3_ > 3 ||
        channel_1_ == channel_2_ ||
        channel_1_ == channel_3_ ||
        channel_2_ == channel_3_
    )
    {
        RCLCPP_ERROR(get_logger(),
                     "channel_1/channel_2/channel_3 must be distinct indices in [0, 3], got %d, %d and %d",
                     channel_1_, channel_2_, channel_3_);
        throw std::invalid_argument("invalid channel indices");
    }
    if (l_ <= 0.0)
    {
        RCLCPP_ERROR(get_logger(), "l must be positive, got %.3f", l_);
        throw std::invalid_argument("l must be positive");
    }

    if (calibration_duration < 0.0)
    {
        calibration_duration = 0;
    }

    calibration_done = (calibration_duration <= 0.0);
    calib_count = 0;
    sum_d1 = 0.0;
    sum_d2 = 0.0;
    sum_d3 = 0.0;

    const std::string topic = declare_parameter<std::string>(
        "subscribe_topic", "/get_serial_msg/distances");
    sub_ = create_subscription<get_serial_msg::msg::Dt35Distances>(
        topic, rclcpp::QoS(10),
        std::bind(&LocalizationNode::distancesCallback, this, std::placeholders::_1));

    pub_ = create_publisher<msg::Localization>("~/localization", rclcpp::QoS(10));

    RCLCPP_INFO(get_logger(),
                "Subscribing to '%s', channels %d/%d/%d, baseline L=%.3f m",
                topic.c_str(), channel_1_, channel_2_, channel_3_, l_);
}

void LocalizationNode::distancesCallback(const get_serial_msg::msg::Dt35Distances::SharedPtr msg)
{
    // 三个通道都有效才解算；否则跳过本帧，保持上一次发布的结果。
    if 
    (
        !msg->valid[channel_1_] || 
        !msg->valid[channel_2_] ||
        !msg->valid[channel_3_]
    )
    {
        RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                             "channel data invalid, skip this frame");
        return;
    }

    const double d1 = msg->distance[channel_1_];
    const double d2 = msg->distance[channel_2_];
    const double d3 = msg->distance[channel_3_];

    if (!calibration_done)
    {    
        const rclcpp::Time now = this->now();
        if (calib_count == 0)
        {
            calib_start = now;
        }

        sum_d1 += d1;
        sum_d2 += d2;
        sum_d3 += d3;
        calib_count ++;

        calibration_done = (
            now - calib_start >= 
            rclcpp::Duration::from_seconds(calibration_duration)
        );
    }

    double ave_d1 = (calib_count > 0) ? sum_d1 / calib_count : d1;
    double ave_d2 = (calib_count > 0) ? sum_d2 / calib_count : d2;
    double ave_d3 = (calib_count > 0) ? sum_d3 / calib_count : d3; 

    publish(msg, ave_d1, ave_d2, ave_d3);
}

void LocalizationNode::publish(
    const get_serial_msg::msg::Dt35Distances::SharedPtr & msg,
    double ave_d1, double ave_d2, double ave_d3)
{
    msg::Localization out;
    const double d1 = msg->distance[channel_1_];
    const double d2 = msg->distance[channel_2_];
    const double d3 = msg->distance[channel_3_];
    out.header = msg->header;
    out.distance = msg->distance;
    out.valid = msg->valid;
    out.channel_1 = channel_1_;
    out.channel_2 = channel_2_;
    out.channel_3 = channel_3_;
    out.cur_d1 = static_cast<float>(d1);
    out.cur_d2 = static_cast<float>(d2);
    out.cur_d3 = static_cast<float>(d3);
    out.ave_d1 = static_cast<float>(ave_d1 + bias_ch_1);
    out.ave_d2 = static_cast<float>(ave_d2 + bias_ch_2);
    out.ave_d3 = static_cast<float>(ave_d3 + bias_ch_3);

    const double dd = ave_d1 + bias_ch_1 - ave_d2 - bias_ch_2;
    const double theta = std::atan2(dd, l_);
    const double yaw = theta - bias_yaw;

    out.d_x = static_cast<float>(
        (ave_d1 + bias_ch_1 + ave_d2 + bias_ch_2) / 2.0 * std::cos(theta)+ len_x * std::cos(yaw)
    );
    out.d_y = static_cast<float>(
        (ave_d3 + bias_ch_3) * std::cos(yaw + bias_d3_alpha) +
        len_y * std::cos(yaw) + 
        (
            (ave_d3 + bias_ch_3) * std::sin(yaw + bias_d3_alpha) +
            out.d_x +
            len_y * std::sin(yaw)
        ) * std::tan(bias_Xaxis)
    );
    out.yaw = static_cast<float>(yaw);

    pub_->publish(out);
}

}  // namespace localization
