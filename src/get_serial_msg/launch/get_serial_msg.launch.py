from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    port_arg = DeclareLaunchArgument(
        'port', default_value='/dev/ttyUSB0',
        description='Serial port the DT35 report link is connected to')
    baud_rate_arg = DeclareLaunchArgument(
        'baud_rate', default_value='115200',
        description='Serial baud rate (MCU firmware uses 115200 8N1)')
    publish_rate_arg = DeclareLaunchArgument(
        'publish_rate', default_value='100.0',
        description='Rate at which the latest decoded frame is published (Hz)')

    get_serial_msg_node = Node(
        package='get_serial_msg',
        executable='get_serial_msg_node',
        name='get_serial_msg',
        output='screen',
        parameters=[{
            'port': LaunchConfiguration('port'),
            'baud_rate': LaunchConfiguration('baud_rate'),
            'publish_rate': LaunchConfiguration('publish_rate'),
        }],
    )

    return LaunchDescription([
        port_arg,
        baud_rate_arg,
        publish_rate_arg,
        get_serial_msg_node,
    ])
