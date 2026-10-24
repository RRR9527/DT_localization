#include <memory>

#include "rclcpp/rclcpp.hpp"

#include "localization/localization_node.hpp"

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<localization::LocalizationNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
