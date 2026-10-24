#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "get_serial_msg/get_serial_msg.hpp"

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<get_serial_msg::GetSerialMsg>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
