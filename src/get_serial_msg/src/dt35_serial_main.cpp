#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "dt_localization/dt35_serial_node.hpp"

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<dt_localization::Dt35SerialNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
