// generated from rosidl_typesupport_fastrtps_cpp/resource/idl__rosidl_typesupport_fastrtps_cpp.hpp.em
// with input from dt_localization:msg/Dt35Distances.idl
// generated code does not contain a copyright notice

#ifndef DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
#define DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_

#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_interface/macros.h"
#include "dt_localization/msg/rosidl_typesupport_fastrtps_cpp__visibility_control.h"
#include "dt_localization/msg/detail/dt35_distances__struct.hpp"

#ifndef _WIN32
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wunused-parameter"
# ifdef __clang__
#  pragma clang diagnostic ignored "-Wdeprecated-register"
#  pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
# endif
#endif
#ifndef _WIN32
# pragma GCC diagnostic pop
#endif

#include "fastcdr/Cdr.h"

namespace dt_localization
{

namespace msg
{

namespace typesupport_fastrtps_cpp
{

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_dt_localization
cdr_serialize(
  const dt_localization::msg::Dt35Distances & ros_message,
  eprosima::fastcdr::Cdr & cdr);

bool
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_dt_localization
cdr_deserialize(
  eprosima::fastcdr::Cdr & cdr,
  dt_localization::msg::Dt35Distances & ros_message);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_dt_localization
get_serialized_size(
  const dt_localization::msg::Dt35Distances & ros_message,
  size_t current_alignment);

size_t
ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_dt_localization
max_serialized_size_Dt35Distances(
  bool & full_bounded,
  bool & is_plain,
  size_t current_alignment);

}  // namespace typesupport_fastrtps_cpp

}  // namespace msg

}  // namespace dt_localization

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_FASTRTPS_CPP_PUBLIC_dt_localization
const rosidl_message_type_support_t *
  ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_fastrtps_cpp, dt_localization, msg, Dt35Distances)();

#ifdef __cplusplus
}
#endif

#endif  // DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__ROSIDL_TYPESUPPORT_FASTRTPS_CPP_HPP_
