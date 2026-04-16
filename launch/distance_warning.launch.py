from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch_ros.actions import Node
from launch.substitutions import LaunchConfiguration

def generate_launch_description():
    # Khai báo argument cho ngưỡng khoảng cách (mặc định 0.5m)
    threshold_arg = DeclareLaunchArgument(
        'threshold',
        default_value='0.5',
        description='Ngưỡng khoảng cách cảnh báo (meters)'
    )
    
    # Khai báo argument riêng cho TF Listener (mặc định 1.0m theo yêu cầu BTVN_02)
    tf_threshold_arg = DeclareLaunchArgument(
        'tf_threshold',
        default_value='1.0',
        description='Ngưỡng để TF listener gọi service'
    )

    threshold = LaunchConfiguration('threshold')
    tf_threshold = LaunchConfiguration('tf_threshold')

    return LaunchDescription([
        threshold_arg,
        tf_threshold_arg,

        # --- NHÓM NODE BTVN_01 ---
        Node(
            package='distance_warning',
            executable='distance_publisher',
            name='distance_publisher',
            output='screen'
        ),
        Node(
            package='distance_warning',
            executable='distance_listener',
            name='distance_listener',
            output='screen',
            parameters=[{'threshold': threshold}]
        ),
        Node(
            package='distance_warning',
            executable='set_threshold_service',
            name='set_threshold_service',
            output='screen',
            parameters=[{'threshold': threshold}]
        ),
        Node(
            package='distance_warning',
            executable='distance_action_server',
            name='distance_action_server',
            output='screen',
            parameters=[{'threshold': threshold}]
        ),
        Node(
            package='distance_warning',
            executable='distance_action_client',
            name='distance_action_client',
            output='screen'
        ),

        # --- NHÓM NODE BTVN_02 (TF2 & QoS) ---
        # Node phát dữ liệu với 2 mức QoS khác nhau (1Hz và 10Hz)
        Node(
            package='distance_warning',
            executable='distance_publisher_qos',
            name='distance_publisher_qos',
            output='screen'
        ),
        # Node nhận dữ liệu và thống kê sai lệch QoS
        Node(
            package='distance_warning',
            executable='distance_listener_qos',
            name='distance_listener_qos',
            output='screen',
            parameters=[{'threshold': threshold}]
        ),
        # Node phát khung tọa độ base_link -> sensor_link dựa trên khoảng cách
        Node(
            package='distance_warning',
            executable='distance_tf_broadcaster',
            name='distance_tf_broadcaster',
            output='screen'
        ),
        # Node lắng nghe tọa độ world -> sensor_link và tự động gọi service
        Node(
            package='distance_warning',
            executable='distance_tf_listener',
            name='distance_tf_listener',
            output='screen',
            parameters=[{'tf_threshold': tf_threshold}]
        ),
    ])