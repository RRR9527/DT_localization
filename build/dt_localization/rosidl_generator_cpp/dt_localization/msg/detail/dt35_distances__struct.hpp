// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from dt_localization:msg/Dt35Distances.idl
// generated code does not contain a copyright notice

#ifndef DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__STRUCT_HPP_
#define DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__dt_localization__msg__Dt35Distances __attribute__((deprecated))
#else
# define DEPRECATED__dt_localization__msg__Dt35Distances __declspec(deprecated)
#endif

namespace dt_localization
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Dt35Distances_
{
  using Type = Dt35Distances_<ContainerAllocator>;

  explicit Dt35Distances_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<float, 4>::iterator, float>(this->distance.begin(), this->distance.end(), 0.0f);
      std::fill<typename std::array<bool, 4>::iterator, bool>(this->valid.begin(), this->valid.end(), false);
      this->frame_count = 0ul;
      this->crc_error_count = 0ul;
    }
  }

  explicit Dt35Distances_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init),
    distance(_alloc),
    valid(_alloc)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      std::fill<typename std::array<float, 4>::iterator, float>(this->distance.begin(), this->distance.end(), 0.0f);
      std::fill<typename std::array<bool, 4>::iterator, bool>(this->valid.begin(), this->valid.end(), false);
      this->frame_count = 0ul;
      this->crc_error_count = 0ul;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _distance_type =
    std::array<float, 4>;
  _distance_type distance;
  using _valid_type =
    std::array<bool, 4>;
  _valid_type valid;
  using _frame_count_type =
    uint32_t;
  _frame_count_type frame_count;
  using _crc_error_count_type =
    uint32_t;
  _crc_error_count_type crc_error_count;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__distance(
    const std::array<float, 4> & _arg)
  {
    this->distance = _arg;
    return *this;
  }
  Type & set__valid(
    const std::array<bool, 4> & _arg)
  {
    this->valid = _arg;
    return *this;
  }
  Type & set__frame_count(
    const uint32_t & _arg)
  {
    this->frame_count = _arg;
    return *this;
  }
  Type & set__crc_error_count(
    const uint32_t & _arg)
  {
    this->crc_error_count = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    dt_localization::msg::Dt35Distances_<ContainerAllocator> *;
  using ConstRawPtr =
    const dt_localization::msg::Dt35Distances_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<dt_localization::msg::Dt35Distances_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<dt_localization::msg::Dt35Distances_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      dt_localization::msg::Dt35Distances_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<dt_localization::msg::Dt35Distances_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      dt_localization::msg::Dt35Distances_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<dt_localization::msg::Dt35Distances_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<dt_localization::msg::Dt35Distances_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<dt_localization::msg::Dt35Distances_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__dt_localization__msg__Dt35Distances
    std::shared_ptr<dt_localization::msg::Dt35Distances_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__dt_localization__msg__Dt35Distances
    std::shared_ptr<dt_localization::msg::Dt35Distances_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Dt35Distances_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->distance != other.distance) {
      return false;
    }
    if (this->valid != other.valid) {
      return false;
    }
    if (this->frame_count != other.frame_count) {
      return false;
    }
    if (this->crc_error_count != other.crc_error_count) {
      return false;
    }
    return true;
  }
  bool operator!=(const Dt35Distances_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Dt35Distances_

// alias to use template instance with default allocator
using Dt35Distances =
  dt_localization::msg::Dt35Distances_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace dt_localization

#endif  // DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__STRUCT_HPP_
