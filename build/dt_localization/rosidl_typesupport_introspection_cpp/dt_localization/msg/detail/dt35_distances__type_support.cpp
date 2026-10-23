// generated from rosidl_typesupport_introspection_cpp/resource/idl__type_support.cpp.em
// with input from dt_localization:msg/Dt35Distances.idl
// generated code does not contain a copyright notice

#include "array"
#include "cstddef"
#include "string"
#include "vector"
#include "rosidl_runtime_c/message_type_support_struct.h"
#include "rosidl_typesupport_cpp/message_type_support.hpp"
#include "rosidl_typesupport_interface/macros.h"
#include "dt_localization/msg/detail/dt35_distances__struct.hpp"
#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/message_type_support_decl.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

namespace dt_localization
{

namespace msg
{

namespace rosidl_typesupport_introspection_cpp
{

void Dt35Distances_init_function(
  void * message_memory, rosidl_runtime_cpp::MessageInitialization _init)
{
  new (message_memory) dt_localization::msg::Dt35Distances(_init);
}

void Dt35Distances_fini_function(void * message_memory)
{
  auto typed_message = static_cast<dt_localization::msg::Dt35Distances *>(message_memory);
  typed_message->~Dt35Distances();
}

size_t size_function__Dt35Distances__distance(const void * untyped_member)
{
  (void)untyped_member;
  return 4;
}

const void * get_const_function__Dt35Distances__distance(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<float, 4> *>(untyped_member);
  return &member[index];
}

void * get_function__Dt35Distances__distance(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<float, 4> *>(untyped_member);
  return &member[index];
}

void fetch_function__Dt35Distances__distance(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const float *>(
    get_const_function__Dt35Distances__distance(untyped_member, index));
  auto & value = *reinterpret_cast<float *>(untyped_value);
  value = item;
}

void assign_function__Dt35Distances__distance(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<float *>(
    get_function__Dt35Distances__distance(untyped_member, index));
  const auto & value = *reinterpret_cast<const float *>(untyped_value);
  item = value;
}

size_t size_function__Dt35Distances__valid(const void * untyped_member)
{
  (void)untyped_member;
  return 4;
}

const void * get_const_function__Dt35Distances__valid(const void * untyped_member, size_t index)
{
  const auto & member =
    *reinterpret_cast<const std::array<bool, 4> *>(untyped_member);
  return &member[index];
}

void * get_function__Dt35Distances__valid(void * untyped_member, size_t index)
{
  auto & member =
    *reinterpret_cast<std::array<bool, 4> *>(untyped_member);
  return &member[index];
}

void fetch_function__Dt35Distances__valid(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const auto & item = *reinterpret_cast<const bool *>(
    get_const_function__Dt35Distances__valid(untyped_member, index));
  auto & value = *reinterpret_cast<bool *>(untyped_value);
  value = item;
}

void assign_function__Dt35Distances__valid(
  void * untyped_member, size_t index, const void * untyped_value)
{
  auto & item = *reinterpret_cast<bool *>(
    get_function__Dt35Distances__valid(untyped_member, index));
  const auto & value = *reinterpret_cast<const bool *>(untyped_value);
  item = value;
}

static const ::rosidl_typesupport_introspection_cpp::MessageMember Dt35Distances_message_member_array[5] = {
  {
    "header",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    ::rosidl_typesupport_introspection_cpp::get_message_type_support_handle<std_msgs::msg::Header>(),  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(dt_localization::msg::Dt35Distances, header),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "distance",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    4,  // array size
    false,  // is upper bound
    offsetof(dt_localization::msg::Dt35Distances, distance),  // bytes offset in struct
    nullptr,  // default value
    size_function__Dt35Distances__distance,  // size() function pointer
    get_const_function__Dt35Distances__distance,  // get_const(index) function pointer
    get_function__Dt35Distances__distance,  // get(index) function pointer
    fetch_function__Dt35Distances__distance,  // fetch(index, &value) function pointer
    assign_function__Dt35Distances__distance,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "valid",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    true,  // is array
    4,  // array size
    false,  // is upper bound
    offsetof(dt_localization::msg::Dt35Distances, valid),  // bytes offset in struct
    nullptr,  // default value
    size_function__Dt35Distances__valid,  // size() function pointer
    get_const_function__Dt35Distances__valid,  // get_const(index) function pointer
    get_function__Dt35Distances__valid,  // get(index) function pointer
    fetch_function__Dt35Distances__valid,  // fetch(index, &value) function pointer
    assign_function__Dt35Distances__valid,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "frame_count",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(dt_localization::msg::Dt35Distances, frame_count),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  },
  {
    "crc_error_count",  // name
    ::rosidl_typesupport_introspection_cpp::ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    nullptr,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(dt_localization::msg::Dt35Distances, crc_error_count),  // bytes offset in struct
    nullptr,  // default value
    nullptr,  // size() function pointer
    nullptr,  // get_const(index) function pointer
    nullptr,  // get(index) function pointer
    nullptr,  // fetch(index, &value) function pointer
    nullptr,  // assign(index, value) function pointer
    nullptr  // resize(index) function pointer
  }
};

static const ::rosidl_typesupport_introspection_cpp::MessageMembers Dt35Distances_message_members = {
  "dt_localization::msg",  // message namespace
  "Dt35Distances",  // message name
  5,  // number of fields
  sizeof(dt_localization::msg::Dt35Distances),
  Dt35Distances_message_member_array,  // message members
  Dt35Distances_init_function,  // function to initialize message memory (memory has to be allocated)
  Dt35Distances_fini_function  // function to terminate message instance (will not free memory)
};

static const rosidl_message_type_support_t Dt35Distances_message_type_support_handle = {
  ::rosidl_typesupport_introspection_cpp::typesupport_identifier,
  &Dt35Distances_message_members,
  get_message_typesupport_handle_function,
};

}  // namespace rosidl_typesupport_introspection_cpp

}  // namespace msg

}  // namespace dt_localization


namespace rosidl_typesupport_introspection_cpp
{

template<>
ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
get_message_type_support_handle<dt_localization::msg::Dt35Distances>()
{
  return &::dt_localization::msg::rosidl_typesupport_introspection_cpp::Dt35Distances_message_type_support_handle;
}

}  // namespace rosidl_typesupport_introspection_cpp

#ifdef __cplusplus
extern "C"
{
#endif

ROSIDL_TYPESUPPORT_INTROSPECTION_CPP_PUBLIC
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_cpp, dt_localization, msg, Dt35Distances)() {
  return &::dt_localization::msg::rosidl_typesupport_introspection_cpp::Dt35Distances_message_type_support_handle;
}

#ifdef __cplusplus
}
#endif
