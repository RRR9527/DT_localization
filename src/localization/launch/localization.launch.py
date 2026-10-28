import os

from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    config_arg = DeclareLaunchArgument(
        'config',
        default_value=os.path.join(
            get_package_share_directory('localization'),
            'config', 'dt_relocalizer.yaml'),
        description='Path to dt_relocalizer.yaml (the authoritative configuration)')

    localization_node = Node(
        package='localization',
        executable='localization_node',
        name='localization_node',
        output='screen',
        parameters=[LaunchConfiguration('config')],
    )

    return LaunchDescription([
        config_arg,
        localization_node,
    ])
