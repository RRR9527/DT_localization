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
    channel_2_ = declare_parameter<int>("channel_2", 1);
    l_ = declare_parameter<double>("l", 1.0);

    // 通道下标必须是 [0, 3] 内互不相同的两个数。
    if (channel_1_ < 0 || channel_1_ > 3 ||
        channel_2_ < 0 || channel_2_ > 3 ||
        channel_1_ == channel_2_)
    {
        RCLCPP_ERROR(get_logger(),
                     "channel_1/channel_2 must be distinct indices in [0, 3], got %d and %d",
                     channel_1_, channel_2_);
        throw std::invalid_argument("invalid channel indices");
    }
    if (l_ <= 0.0)
    {
        RCLCPP_ERROR(get_logger(), "l must be positive, got %.3f", l_);
        throw std::invalid_argument("l must be positive");
    }

    const std::string topic = declare_parameter<std::string>(
        "subscribe_topic", "/get_serial_msg/distances");
    sub_ = create_subscription<get_serial_msg::msg::Dt35Distances>(
        topic, rclcpp::QoS(10),
        std::bind(&LocalizationNode::distancesCallback, this, std::placeholders::_1));

    pub_ = create_publisher<msg::Localization>("~/localization", rclcpp::QoS(10));

    RCLCPP_INFO(get_logger(),
                "Subscribing to '%s', channels %d/%d, baseline L=%.3f m",
                topic.c_str(), channel_1_, channel_2_, l_);
}

void LocalizationNode::distancesCallback(const get_serial_msg::msg::Dt35Distances::SharedPtr msg)
{
    // 两个通道都有效才解算；否则跳过本帧，保持上一次发布的结果。
    if (!msg->valid[channel_1_] || !msg->valid[channel_2_])
    {
        RCLCPP_WARN_THROTTLE(get_logger(), *get_clock(), 1000,
                             "channel data invalid, skip this frame");
        return;
    }

    const double d1 = msg->distance[channel_1_];
    const double d2 = msg->distance[channel_2_];

    msg::Localization out;
    out.header = msg->header;
    // 透传全部 4 个通道的原始数据，剩下两个通道留作以后扩展使用。
    out.distance = msg->distance;
    out.valid = msg->valid;
    out.channel_1 = channel_1_;
    out.channel_2 = channel_2_;
    out.d1 = static_cast<float>(d1);
    out.d2 = static_cast<float>(d2);
    // D = (d1 + d2) / 2
    out.d = static_cast<float>((d1 + d2) / 2.0);
    // yaw = atan((d1 - d2) / L)
    out.yaw = static_cast<float>(std::atan((d1 - d2) / l_));

    pub_->publish(out);
}

}  // namespace localization
