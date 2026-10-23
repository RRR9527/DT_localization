// DT35/DT50 测距上报数据串口接收节点。
//
// 协议（参见 UenoharaAyano/DT35-IOLink，project/APP/sensor_dt35/app_dt35.c）：
//   字节 0  : 0xFF 帧头
//   字节 1  : 0x55 帧头
//   字节 2-9: 8 字节 payload，小端打包
//   字节 10 : CRC-8（多项式 0x07，初值 0x00），校验范围是字节 0..9
//
// payload 位布局：
//   位 0..3                : 有效掩码，第 i 位置 1 表示第 i 路距离已更新
//   位 8+14*i .. 8+14*i+13 : 第 i 路距离，单位 mm（14 位，上限 0x3FFF）

#ifndef DT_LOCALIZATION__DT35_SERIAL_NODE_HPP_
#define DT_LOCALIZATION__DT35_SERIAL_NODE_HPP_

#include <array>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <string>
#include <thread>

#include "rclcpp/rclcpp.hpp"

#include "dt_localization/msg/dt35_distances.hpp"

namespace dt_localization
{

class Dt35SerialNode : public rclcpp::Node
{
public:
  explicit Dt35SerialNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());
  ~Dt35SerialNode() override;

private:
  static constexpr uint8_t kHeader1 = 0xFFU;
  static constexpr uint8_t kHeader2 = 0x55U;
  static constexpr size_t kPayloadLen = 8U;   // 2 帧头 + 8 payload + 1 CRC = 11 字节
  static constexpr size_t kPortCount = 4U;
  static constexpr uint16_t kDistanceMaxMm = 0x3FFFU;
  static constexpr size_t kReadBufSize = 256U;

  enum class DecodeState : uint8_t
  {
    kHeader1,
    kHeader2,
    kPayload,
    kCrc,
  };

  bool openSerial(const std::string & port, int baud_rate);
  void closeSerial();
  void readLoop();
  void publishLoop();

  void decodeByte(uint8_t byte);
  void handleFrame(const std::array<uint8_t, kPayloadLen> & payload);
  static uint8_t crc8Update(uint8_t crc, uint8_t byte);

  rclcpp::Publisher<msg::Dt35Distances>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr publish_timer_;

  std::thread read_thread_;
  std::atomic<bool> running_{false};

  int fd_ = -1;
  std::string port_;
  int baud_rate_ = 115200;

  // 解码状态机，仅由读线程访问。
  DecodeState state_ = DecodeState::kHeader1;
  std::array<uint8_t, kPayloadLen> rx_buf_{};
  size_t rx_idx_ = 0U;
  uint8_t rx_crc_ = 0U;

  // 最新解码数据，由 mutex_ 保护。
  std::mutex mutex_;
  msg::Dt35Distances latest_;
  rclcpp::Time last_frame_time_;
  bool frame_received_ = false;
  std::atomic<uint64_t> frame_count_{0U};
  std::atomic<uint64_t> crc_error_count_{0U};
};

}  // namespace dt_localization

#endif  // DT_LOCALIZATION__DT35_SERIAL_NODE_HPP_
