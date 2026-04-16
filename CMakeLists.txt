cmake_minimum_required(VERSION 3.8)
project(distance_warning)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclcpp_action REQUIRED)
find_package(std_msgs REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(tf2 REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(tf2_geometry_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)

rosidl_generate_interfaces(${PROJECT_NAME}
  "srv/SetThreshold.srv"
  "action/CheckDistance.action"
)

add_executable(distance_publisher src/distance_publisher.cpp)
ament_target_dependencies(distance_publisher rclcpp std_msgs tf2 tf2_ros tf2_geometry_msgs geometry_msgs)
install(TARGETS distance_publisher DESTINATION lib/${PROJECT_NAME})

add_executable(distance_listener src/distance_listener.cpp)
ament_target_dependencies(distance_listener rclcpp std_msgs tf2 tf2_ros tf2_geometry_msgs geometry_msgs)
install(TARGETS distance_listener DESTINATION lib/${PROJECT_NAME})

add_executable(set_threshold_service src/set_threshold_service.cpp)
ament_target_dependencies(set_threshold_service rclcpp)
rosidl_target_interfaces(set_threshold_service ${PROJECT_NAME} "rosidl_typesupport_cpp")
install(TARGETS set_threshold_service DESTINATION lib/${PROJECT_NAME})

add_executable(distance_action_server src/distance_action_server.cpp)
ament_target_dependencies(distance_action_server rclcpp rclcpp_action tf2 tf2_ros tf2_geometry_msgs geometry_msgs)
rosidl_target_interfaces(distance_action_server ${PROJECT_NAME} "rosidl_typesupport_cpp")
install(TARGETS distance_action_server DESTINATION lib/${PROJECT_NAME})

add_executable(distance_action_client src/distance_action_client.cpp)
ament_target_dependencies(distance_action_client rclcpp rclcpp_action)
rosidl_target_interfaces(distance_action_client ${PROJECT_NAME} "rosidl_typesupport_cpp")
install(TARGETS distance_action_client DESTINATION lib/${PROJECT_NAME})

add_executable(distance_tf_broadcaster src/distance_tf_broadcaster.cpp)
ament_target_dependencies(distance_tf_broadcaster rclcpp std_msgs geometry_msgs tf2 tf2_ros)
install(TARGETS distance_tf_broadcaster DESTINATION lib/${PROJECT_NAME})


add_executable(distance_tf_listener src/distance_tf_listener.cpp)
ament_target_dependencies(distance_tf_listener rclcpp geometry_msgs tf2 tf2_ros tf2_geometry_msgs)
rosidl_target_interfaces(distance_tf_listener ${PROJECT_NAME} "rosidl_typesupport_cpp")
install(TARGETS distance_tf_listener DESTINATION lib/${PROJECT_NAME})

add_executable(distance_publisher_qos src/distance_publisher_qos.cpp)
ament_target_dependencies(distance_publisher_qos rclcpp std_msgs)
install(TARGETS distance_publisher_qos DESTINATION lib/${PROJECT_NAME})

add_executable(distance_listener_qos src/distance_listener_qos.cpp)
ament_target_dependencies(distance_listener_qos rclcpp std_msgs)
install(TARGETS distance_listener_qos DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY launch DESTINATION share/${PROJECT_NAME})

ament_package()
