# generated from
# rosidl_cmake/cmake/template/rosidl_cmake_export_typesupport_targets.cmake.in

set(_exported_typesupport_targets
  "__rosidl_generator_c:dt_localization__rosidl_generator_c;__rosidl_typesupport_fastrtps_c:dt_localization__rosidl_typesupport_fastrtps_c;__rosidl_generator_cpp:dt_localization__rosidl_generator_cpp;__rosidl_typesupport_fastrtps_cpp:dt_localization__rosidl_typesupport_fastrtps_cpp;__rosidl_typesupport_introspection_c:dt_localization__rosidl_typesupport_introspection_c;__rosidl_typesupport_c:dt_localization__rosidl_typesupport_c;__rosidl_typesupport_introspection_cpp:dt_localization__rosidl_typesupport_introspection_cpp;__rosidl_typesupport_cpp:dt_localization__rosidl_typesupport_cpp;__rosidl_generator_py:dt_localization__rosidl_generator_py")

# populate dt_localization_TARGETS_<suffix>
if(NOT _exported_typesupport_targets STREQUAL "")
  # loop over typesupport targets
  foreach(_tuple ${_exported_typesupport_targets})
    string(REPLACE ":" ";" _tuple "${_tuple}")
    list(GET _tuple 0 _suffix)
    list(GET _tuple 1 _target)

    set(_target "dt_localization::${_target}")
    if(NOT TARGET "${_target}")
      # the exported target must exist
      message(WARNING "Package 'dt_localization' exports the typesupport target '${_target}' which doesn't exist")
    else()
      list(APPEND dt_localization_TARGETS${_suffix} "${_target}")
    endif()
  endforeach()
endif()
