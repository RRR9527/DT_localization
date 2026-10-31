// 三通道测距定位节点。
//
// 订阅 get_serial_msg 发布的 Dt35Distances，取三个通道的距离 d1、d2、d3。
// 开机前 calibration_duration 秒内累计求平均，位姿由平均距离解算：
//   d_x  = ((d1 + d2) / 2 + len_x) * cos(yaw)  —— 到墙面x的距离
//   d_y  = (d3 + len_y) * cos(yaw)             —— 到墙面y的距离
//   yaw  = atan2(d1 - d2, L)                   —— 相对墙面的偏航角
//   cos(yaw) = L / sqrt(L^2 + (d1 - d2)^2)      —— 斜距→垂直距离修正
// L 为两传感器间距，len_x/len_y 为传感器距车中心的偏移，均为待标定参数。
//
// 注意：yaw 的符号约定取决于两个传感器在车上的左右安装顺序，
// 若符号与期望相反，交换 channel_1 / channel_2 两个下标即可。

#ifndef LOCALIZATION__LOCALIZATION_NODE_HPP_
#define LOCALIZATION__LOCALIZATION_NODE_HPP_

#include "rclcpp/rclcpp.hpp"

#include "get_serial_msg/msg/dt35_distances.hpp"
#include "localization/msg/localization.hpp"
#include <cstddef>

namespace localization
{

class LocalizationNode : public rclcpp::Node
{
public:
    explicit LocalizationNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());

private:
    void distancesCallback(const get_serial_msg::msg::Dt35Distances::SharedPtr msg);
    void publish(const get_serial_msg::msg::Dt35Distances::SharedPtr & msg,
             double ave_d1, double ave_d2, double ave_d3);

    rclcpp::Subscription<get_serial_msg::msg::Dt35Distances>::SharedPtr sub_;
    rclcpp::Publisher<msg::Localization>::SharedPtr pub_;

    int channel_1_;   // 通道 1 在 distance[4] 中的下标（从 0 开始），由参数决定
    int channel_2_;   // 通道 2 在 distance[4] 中的下标（从 0 开始），由参数决定
    int channel_3_;   // 通道 3 在 distance[4] 中的下标（从 0 开始），由参数决定
    double l_;        // 两传感器间距 L（m），由参数决定
    double len_x;     // x方向传感器距车中心的距离
    double len_y;     // y方向传感器距车中心的距离

    double calibration_duration; // 初始静止标定时间
    rclcpp::Time calib_start;    // 启始时间戳
    bool calibration_done;       // 是否完成
    double sum_d1;               // d1累计值
    double sum_d2;               // d2累计值
    double sum_d3;               // d3累计值
    std::size_t calib_count;     // 累计样本数
};

}  // namespace localization

#endif  // LOCALIZATION__LOCALIZATION_NODE_HPP_
