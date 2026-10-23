# generated from rosidl_cmake/cmake/rosidl_cmake_aggregate_target-extras.cmake.in

# Create a convenience aggregate target dt_localization::dt_localization
# that links all generated interface targets, so downstream packages can use
# a single modern CMake target name instead of ${dt_localization_TARGETS}.
if(dt_localization_TARGETS AND NOT TARGET dt_localization::dt_localization)
  add_library(dt_localization::dt_localization INTERFACE IMPORTED)
  set_target_properties(dt_localization::dt_localization PROPERTIES
    INTERFACE_LINK_LIBRARIES "${dt_localization_TARGETS}")
endif()
