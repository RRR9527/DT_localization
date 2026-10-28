cmake_minimum_required(VERSION 3.8)
project(localization)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(std_srvs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(get_serial_msg REQUIRED)
find_package(rosidl_default_generators REQUIRED)
find_package(eigen3_cmake_module REQUIRED)
find_package(Eigen3 REQUIRED)

# 定位结果消息
rosidl_generate_interfaces(${PROJECT_NAME}
  "msg/RelocalizationResult.msg"
  DEPENDENCIES std_msgs
)

add_executable(localization_node
  src/distance_adapter.cpp
  src/relocalizer.cpp
  src/localization_node.cpp
  src/localization_node_main.cpp
)
target_include_directories(localization_node PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
target_compile_features(localization_node PUBLIC cxx_std_17)
# get_serial_msg 的消息库通过 ament_target_dependencies 一起链接进来。
ament_target_dependencies(localization_node
  rclcpp std_msgs std_srvs geometry_msgs get_serial_msg)
rosidl_get_typesupport_target(localization_cpp_ts ${PROJECT_NAME} "rosidl_typesupport_cpp")
target_link_libraries(localization_node "${localization_cpp_ts}" Eigen3::Eigen)

install(TARGETS localization_node
  DESTINATION lib/${PROJECT_NAME})
install(DIRECTORY include/
  DESTINATION include)
install(DIRECTORY launch config
  DESTINATION share/${PROJECT_NAME})

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_export_include_directories(include)
ament_export_dependencies(rosidl_default_runtime)
ament_package()
