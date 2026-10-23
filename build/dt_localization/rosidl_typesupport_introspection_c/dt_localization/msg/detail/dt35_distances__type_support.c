// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from dt_localization:msg/Dt35Distances.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "dt_localization/msg/detail/dt35_distances__rosidl_typesupport_introspection_c.h"
#include "dt_localization/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "dt_localization/msg/detail/dt35_distances__functions.h"
#include "dt_localization/msg/detail/dt35_distances__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  dt_localization__msg__Dt35Distances__init(message_memory);
}

void dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_fini_function(void * message_memory)
{
  dt_localization__msg__Dt35Distances__fini(message_memory);
}

size_t dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__size_function__Dt35Distances__distance(
  const void * untyped_member)
{
  (void)untyped_member;
  return 4;
}

const void * dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_const_function__Dt35Distances__distance(
  const void * untyped_member, size_t index)
{
  const float * member =
    (const float *)(untyped_member);
  return &member[index];
}

void * dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_function__Dt35Distances__distance(
  void * untyped_member, size_t index)
{
  float * member =
    (float *)(untyped_member);
  return &member[index];
}

void dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__fetch_function__Dt35Distances__distance(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_const_function__Dt35Distances__distance(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__assign_function__Dt35Distances__distance(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_function__Dt35Distances__distance(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

size_t dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__size_function__Dt35Distances__valid(
  const void * untyped_member)
{
  (void)untyped_member;
  return 4;
}

const void * dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_const_function__Dt35Distances__valid(
  const void * untyped_member, size_t index)
{
  const bool * member =
    (const bool *)(untyped_member);
  return &member[index];
}

void * dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_function__Dt35Distances__valid(
  void * untyped_member, size_t index)
{
  bool * member =
    (bool *)(untyped_member);
  return &member[index];
}

void dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__fetch_function__Dt35Distances__valid(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const bool * item =
    ((const bool *)
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_const_function__Dt35Distances__valid(untyped_member, index));
  bool * value =
    (bool *)(untyped_value);
  *value = *item;
}

void dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__assign_function__Dt35Distances__valid(
  void * untyped_member, size_t index, const void * untyped_value)
{
  bool * item =
    ((bool *)
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_function__Dt35Distances__valid(untyped_member, index));
  const bool * value =
    (const bool *)(untyped_value);
  *item = *value;
}

static rosidl_typesupport_introspection_c__MessageMember dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_message_member_array[5] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(dt_localization__msg__Dt35Distances, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "distance",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    4,  // array size
    false,  // is upper bound
    offsetof(dt_localization__msg__Dt35Distances, distance),  // bytes offset in struct
    NULL,  // default value
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__size_function__Dt35Distances__distance,  // size() function pointer
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_const_function__Dt35Distances__distance,  // get_const(index) function pointer
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_function__Dt35Distances__distance,  // get(index) function pointer
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__fetch_function__Dt35Distances__distance,  // fetch(index, &value) function pointer
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__assign_function__Dt35Distances__distance,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "valid",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    4,  // array size
    false,  // is upper bound
    offsetof(dt_localization__msg__Dt35Distances, valid),  // bytes offset in struct
    NULL,  // default value
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__size_function__Dt35Distances__valid,  // size() function pointer
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_const_function__Dt35Distances__valid,  // get_const(index) function pointer
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__get_function__Dt35Distances__valid,  // get(index) function pointer
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__fetch_function__Dt35Distances__valid,  // fetch(index, &value) function pointer
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__assign_function__Dt35Distances__valid,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "frame_count",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(dt_localization__msg__Dt35Distances, frame_count),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "crc_error_count",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT32,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(dt_localization__msg__Dt35Distances, crc_error_count),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_message_members = {
  "dt_localization__msg",  // message namespace
  "Dt35Distances",  // message name
  5,  // number of fields
  sizeof(dt_localization__msg__Dt35Distances),
  dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_message_member_array,  // message members
  dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_init_function,  // function to initialize message memory (memory has to be allocated)
  dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_message_type_support_handle = {
  0,
  &dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_dt_localization
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, dt_localization, msg, Dt35Distances)() {
  dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  if (!dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_message_type_support_handle.typesupport_identifier) {
    dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &dt_localization__msg__Dt35Distances__rosidl_typesupport_introspection_c__Dt35Distances_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
