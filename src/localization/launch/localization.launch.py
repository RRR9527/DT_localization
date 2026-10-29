from launch import LaunchDescription
from launch.substitutions import PathJoinSubstitution
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare


def generate_launch_description():
    # 参数统一从 config/localization.yaml 读取，不要在代码或 launch 里写死数值。
    params_file = PathJoinSubstitution([
        FindPackageShare('localization'),
        'config',
        'localization.yaml',
    ])

    localization_node = Node(
        package='localization',
        executable='localization_node',
        name='localization_node',
        output='screen',
        parameters=[params_file],
    )

    return LaunchDescription([
        localization_node,
    ])
