#include "dt_localization/dt35_serial_node.hpp"

#include <cerrno>
#include <chrono>
#include <cstring>
#include <functional>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

namespace dt_localization
{

namespace
{

// 将波特率数值映射为 termios 的速度常量（B 系列宏）。
speed_t mapBaudRate(int baud_rate)
{
    switch (baud_rate) 
    {
        case 9600  : return B9600;
        case 19200 : return B19200;
        case 38400 : return B38400;
        case 57600 : return B57600;
        case 115200: return B115200;
        case 230400: return B230400;
        case 460800: return B460800;
        case 921600: return B921600;
        default: return B0;
    }
}

}  // namespace

Dt35SerialNode::Dt35SerialNode(const rclcprclcppp::NodeOptions & options)
: Node("dt35_serial_node", options)
{
    port_ = declare_parameter<std::string>("port", "/dev/ttyUSB0");
    baud_rate_ = declare_parameter<int>("baud_rate", 115200);
    const double publish_rate = declare_parameter<double>("publish_rate", 100.0);
    const std::string frame_id = declare_parameter<std::string>("frame_id", "dt35_link");

    latest_.header.frame_id = frame_id;
    latest_.distance.fill(0.0F);
    latest_.valid.fill(false);
    latest_.frame_count = 0U;
    latest_.crc_error_count = 0U;
    // 可靠 + keep-last QoS，让普通订阅者（ros2 topic echo、默认 QoS）
    // 无需额外设置即可匹配。
    pub_ = create_publisher<msg::Dt35Distances>("~/distances", rclcpp::QoS(10));

    // 读线程负责收串口字节并解码；定时器按固定频率发布最新结果。
    running_ = true;
    read_thread_ = std::thread(&Dt35SerialNode::readLoop, this);

    const auto period = std::chrono::duration_cast<std::chrono::nanoseconds>(
      std::chrono::duration<double>(1.0 / publish_rate));
    publish_timer_ = create_wall_timer(
      period, std::bind(&Dt35SerialNode::publishLoop, this));

    RCLCPP_INFO(
      get_logger(), "Listening on %s at %d baud, publishing to '%s' at %.1f Hz",
      port_.c_str(), baud_rate_, pub_->get_topic_name(), publish_rate);
}

Dt35SerialNode::~Dt35SerialNode()
{
    running_ = false;
    if (read_thread_.joinable()) 
    {
        read_thread_.join();
    }
    closeSerial();
}

bool Dt35SerialNode::openSerial(const std::string & port, int baud_rate)
{
    const speed_t speed = mapBaudRate(baud_rate);
    if (speed == B0) 
    {
        RCLCPP_ERROR(get_logger(), "Unsupported baud rate: %d", baud_rate);
        return false;
    }

    const int fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fd < 0) 
    {
        RCLCPP_ERROR(
            get_logger(), "Failed to open %s: %s", port.c_str(), std::strerror(errno));
        return false;
    }

    // 改回阻塞读取，等待上限由下面的 termios 超时控制。
    const int flags = fcntl(fd, F_GETFL, 0);
    if (flags >= 0) 
    {
        fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
    }

    termios tty{};
    if (tcgetattr(fd, &tty) != 0) 
    {
        RCLCPP_ERROR(
            get_logger(), "tcgetattr(%s) failed: %s", port.c_str(), std::strerror(errno));
        close(fd);
        return false;
    }

    cfmakeraw(&tty);
    tty.c_cflag |= CLOCAL | CREAD;
    tty.c_cflag &= ~(CSIZE | PARENB | CSTOPB);
    tty.c_cflag |= CS8;  // 8N1，与 MCU 的 USART2 一致
    cfsetispeed(&tty, speed);
    cfsetospeed(&tty, speed);
    // VMIN=0、VTIME=1：无数据时 read() 最多阻塞 100 ms 即超时返回，
    // 读线程既能及时处理数据，也能在退出/重连时快速响应。
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1;  // read() 最多等待 100 ms 即返回

    if (tcsetattr(fd, TCSANOW, &tty) != 0) 
    {
        RCLCPP_ERROR(
            get_logger(), "tcsetattr(%s) failed: %s", port.c_str(), std::strerror(errno));
        close(fd);
        return false;
    }
    tcflush(fd, TCIOFLUSH);

    fd_ = fd;
    // （重）连接后重置解码器，重新同步帧头。
    state_ = DecodeState::kHeader1;
    rx_idx_ = 0U;
    rx_crc_ = 0U;

    RCLCPP_INFO(get_logger(), "Serial port %s opened at %d baud", port.c_str(), baud_rate);
    return true;
}

void Dt35SerialNode::closeSerial()
{
    if (fd_ >= 0) 
    {
      close(fd_);
      fd_ = -1;
    }
}

void Dt35SerialNode::readLoop()
{
    std::array<uint8_t, kReadBufSize> buf{};

    while (running_ && rclcpp::ok()) 
    {
        if (fd_ < 0) 
        {
            // 首次连接或设备拔出后的重连。
            if (!openSerial(port_, baud_rate_))
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            continue;
        }

        // 阻塞读取（最多 100 ms 超时），把读到的字节逐个交给解码器。
        const ssize_t n = read(fd_, buf.data(), buf.size());
        if (n > 0) 
        {
            for (ssize_t i = 0; i < n; ++i) 
            {
                decodeByte(buf[static_cast<size_t>(i)]);
            }
        } 
        else if (n < 0) 
        {
            if (errno == EINTR || errno == EAGAIN) 
            {
                continue;
            }
            RCLCPP_WARN_THROTTLE(
                get_logger(), *get_clock(), 5000,
                "Serial read failed: %s; will try to reopen", std::strerror(errno));
            closeSerial();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        // n == 0：读取超时，继续循环。
    }
}

void Dt35SerialNode::publishLoop()
{
    // 定时发布最新一帧；数据由读线程更新，此处加锁读取。
    std::lock_guard<std::mutex> lock(mutex_);
    if (!frame_received_) 
    {
        return;
    }
    msg::Dt35Distances out = latest_;
    out.header.stamp = last_frame_time_;
    pub_->publish(out);
}

uint8_t Dt35SerialNode::crc8Update(uint8_t crc, uint8_t byte)
{
    // CRC-8（多项式 0x07，初值 0x00），与发送端 DT35_ReportCrc8 一致。
    crc ^= byte;
    for (int i = 0; i < 8; ++i) 
    {
        if ((crc & 0x80U) != 0U) 
        {
            crc = static_cast<uint8_t>((crc << 1) ^ 0x07U);
        } 
        else 
        {
            crc = static_cast<uint8_t>(crc << 1);
        }
    }
    return crc;
}

void Dt35SerialNode::decodeByte(uint8_t byte)
{
    // 四态状态机：帧头1 -> 帧头2 -> 8 字节 payload -> CRC 校验字节。
    // 任一字节不符合预期都会回退，从而自动与流重新同步。
    switch (state_) 
    {
        case DecodeState::kHeader1:
            if (byte == kHeader1) 
            {
                rx_crc_ = crc8Update(0U, byte);
                state_ = DecodeState::kHeader2;
            }
            break;

        case DecodeState::kHeader2:
            if (byte == kHeader2) 
            {
                rx_crc_ = crc8Update(rx_crc_, byte);
                rx_idx_ = 0U;
                state_ = DecodeState::kPayload;
            } else 
            {
                state_ = DecodeState::kHeader1;
            }
            break;

        case DecodeState::kPayload:
            rx_buf_[rx_idx_++] = byte;
            rx_crc_ = crc8Update(rx_crc_, byte);
            if (rx_idx_ >= kPayloadLen) 
            {
                state_ = DecodeState::kCrc;
            }
            break;

        case DecodeState::kCrc:
            if (byte == rx_crc_) 
            {
                handleFrame(rx_buf_);
            } 
            else 
            {
                crc_error_count_++;
                RCLCPP_WARN_THROTTLE(
                    get_logger(), *get_clock(), 5000, "CRC mismatch (%llu so far)",
                    static_cast<unsigned long long>(crc_error_count_.load()));
            }
            state_ = DecodeState::kHeader1;
            break;
    }
}

void Dt35SerialNode::handleFrame(const std::array<uint8_t, kPayloadLen> & payload)
{
    // 8 字节 payload 按小端拼成 64 位整数，便于按位取字段。
    uint64_t packed = 0U;
    for (size_t i = 0U; i < kPayloadLen; ++i) 
    {
        packed |= static_cast<uint64_t>(payload[i]) << (8U * i);
    }

    // 低 4 位是有效掩码；第 i 路距离位于位 8+14*i 起的 14 位（单位 mm）。
    const uint8_t valid_mask = payload[0] & 0x0FU;
    const rclcpp::Time stamp = now();

    std::lock_guard<std::mutex> lock(mutex_);
    for (size_t i = 0U; i < kPortCount; ++i) 
    {
      if ((valid_mask & (1U << i)) != 0U) 
      {
          const uint16_t distance_mm = static_cast<uint16_t>(
              (packed >> (8U + 14U * i)) & static_cast<uint64_t>(kDistanceMaxMm));
          latest_.distance[i] = static_cast<float>(distance_mm) * 0.001F;
          latest_.valid[i] = true;
      } 
      else 
      {
          latest_.valid[i] = false;
          // 该路保持上一次的距离值不变。
      }
    }
    latest_.frame_count = static_cast<uint32_t>(frame_count_.fetch_add(1U) + 1U);
    latest_.crc_error_count = static_cast<uint32_t>(crc_error_count_.load());
    last_frame_time_ = stamp;
    frame_received_ = true;
}

}  // namespace dt_localization
