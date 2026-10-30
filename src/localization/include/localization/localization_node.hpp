// 三通道测距定位节点。
//
// 订阅 get_serial_msg 发布的 Dt35Distances，取其中三个通道的距离 d1、d2：
//   d_x = (d1 + d2) / 2        —— 到墙面x的平均距离
//   d_y = \frac{d3 l}{\sqrt{l^2 + (d1 - d2)^2}} + len_y
//                              —— 到墙面y的平均距离
//   yaw = atan((d1 - d2) / L)  —— 相对墙面的偏航角
// L 为两传感器间距，是待标定的参数。
// 其余通道的原始数据原样透传到 Localization 消息里，供后续扩展使用。
//
// 注意：yaw 的符号约定取决于两个传感器在车上的左右安装顺序，
// 若符号与期望相反，交换 channel_1 / channel_2 两个下标即可。

#ifndef LOCALIZATION__LOCALIZATION_NODE_HPP_
#define LOCALIZATION__LOCALIZATION_NODE_HPP_

#include "rclcpp/rclcpp.hpp"

#include "get_serial_msg/msg/dt35_distances.hpp"
#include "localization/msg/localization.hpp"

namespace localization
{

class LocalizationNode : public rclcpp::Node
{
public:
    explicit LocalizationNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

private:
    void distancesCallback(const get_serial_msg::msg::Dt35Distances::SharedPtr msg);

    rclcpp::Subscription<get_serial_msg::msg::Dt35Distances>::SharedPtr sub_;
    rclcpp::Publisher<msg::Localization>::SharedPtr pub_;

    int channel_1_;   // 通道 1 在 distance[4] 中的下标（从 0 开始），由参数决定
    int channel_2_;   // 通道 2 在 distance[4] 中的下标（从 0 开始），由参数决定
    int channel_3_;   // 通道 3 在 distance[4] 中的下标（从 0 开始），由参数决定
    double l_;        // 两传感器间距 L（m），由参数决定
    double len_x;     // x方向传感器距车中心的距离
    double len_y;     // y方向传感器距车中心的距离
};

}  // namespace localization

#endif  // LOCALIZATION__LOCALIZATION_NODE_HPP_
