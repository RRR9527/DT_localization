// NOLINT: This file starts with a BOM since it contain non-ASCII characters
// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from dt_localization:msg/Dt35Distances.idl
// generated code does not contain a copyright notice

#ifndef DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__STRUCT_H_
#define DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"

/// Struct defined in msg/Dt35Distances in the package dt_localization.
typedef struct dt_localization__msg__Dt35Distances
{
  std_msgs__msg__Header header;
  /// 四个测距端口的距离，单位米。
  /// 端口会保留上一次有效读数；`valid` 指示哪些端口在本帧中被更新。
  float distance[4];
  bool valid[4];
  /// 自节点启动以来的诊断计数。
  /// 成功解码的帧数
  uint32_t frame_count;
  /// 因 CRC 校验失败被丢弃的帧数
  uint32_t crc_error_count;
} dt_localization__msg__Dt35Distances;

// Struct for a sequence of dt_localization__msg__Dt35Distances.
typedef struct dt_localization__msg__Dt35Distances__Sequence
{
  dt_localization__msg__Dt35Distances * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} dt_localization__msg__Dt35Distances__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__STRUCT_H_
