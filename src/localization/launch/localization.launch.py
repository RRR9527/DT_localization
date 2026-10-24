from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    channel_1_arg = DeclareLaunchArgument(
        'channel_1', default_value='0',
        description='Index of channel 1 in Dt35Distances.distance (0-based)')
    channel_2_arg = DeclareLaunchArgument(
        'channel_2', default_value='1',
        description='Index of channel 2 in Dt35Distances.distance (0-based)')
    l_arg = DeclareLaunchArgument(
        'l', default_value='1.0',
        description='Baseline L between the two ranging sensors (m), TODO: calibrate')

    localization_node = Node(
        package='localization',
        executable='localization_node',
        name='localization_node',
        output='screen',
        parameters=[{
            'channel_1': LaunchConfiguration('channel_1'),
            'channel_2': LaunchConfiguration('channel_2'),
            'l': LaunchConfiguration('l'),
        }],
    )

    return LaunchDescription([
        channel_1_arg,
        channel_2_arg,
        l_arg,
        localization_node,
    ])
