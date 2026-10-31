// 三通道测距定位节点。
//
// 订阅 get_serial_msg 发布的 Dt35Distances，取三个通道的距离 d1、d2、d3。
// 开机前 calibration_duration 秒内累计求平均，位姿由修正后的平均距离解算：
//   d1c = ave_d1 + bias_ch_1（d2c、d3c 同理；bias 约定：真值 = 读数 + bias）
//   theta = atan2(d1c - d2c, L)      —— 两传感器基线相对墙面的角度
//   yaw   = theta - bias_yaw         —— 车身相对墙面的偏航角
//   d_x   = (d1c + d2c) / 2 * cos(theta) + len_x * cos(yaw)  —— 到墙面x的距离
//   d_y   = d3c * cos(yaw + bias_d3_alpha)
//         + len_y * cos(yaw)
//         + [d3c * sin(yaw + bias_d3_alpha) + d_x + len_y * sin(yaw)] * tan(bias_Xaxis)
//         —— 到墙面y的距离，含 ch3 光轴偏角与墙角非正交修正
// L 为两传感器间距，len_x/len_y 为传感器距车中心的偏移。
// bias_yaw、bias_d3_alpha、bias_Xaxis 单位均为弧度，含义见 localization.yaml。
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

    double bias_ch_1;      // 通道 1 的零偏（m），真值 = 读数 + bias
    double bias_ch_2;      // 通道 2 的零偏（m），真值 = 读数 + bias
    double bias_ch_3;      // 通道 3 的零偏（m），真值 = 读数 + bias
    double bias_d3_alpha;  // 通道 3 光轴安装偏角（rad）
    double bias_yaw;       // 通道 1/2 整体安装歪斜角（rad），假设二者光轴平行
    double bias_Xaxis;     // 墙的夹角偏差（rad）。默认Y轴是正的，墙的夹角偏差由X轴引起
};

}  // namespace localization

#endif  // LOCALIZATION__LOCALIZATION_NODE_HPP_
