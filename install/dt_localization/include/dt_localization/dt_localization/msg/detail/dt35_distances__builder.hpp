// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from dt_localization:msg/Dt35Distances.idl
// generated code does not contain a copyright notice

#ifndef DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__BUILDER_HPP_
#define DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "dt_localization/msg/detail/dt35_distances__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace dt_localization
{

namespace msg
{

namespace builder
{

class Init_Dt35Distances_crc_error_count
{
public:
  explicit Init_Dt35Distances_crc_error_count(::dt_localization::msg::Dt35Distances & msg)
  : msg_(msg)
  {}
  ::dt_localization::msg::Dt35Distances crc_error_count(::dt_localization::msg::Dt35Distances::_crc_error_count_type arg)
  {
    msg_.crc_error_count = std::move(arg);
    return std::move(msg_);
  }

private:
  ::dt_localization::msg::Dt35Distances msg_;
};

class Init_Dt35Distances_frame_count
{
public:
  explicit Init_Dt35Distances_frame_count(::dt_localization::msg::Dt35Distances & msg)
  : msg_(msg)
  {}
  Init_Dt35Distances_crc_error_count frame_count(::dt_localization::msg::Dt35Distances::_frame_count_type arg)
  {
    msg_.frame_count = std::move(arg);
    return Init_Dt35Distances_crc_error_count(msg_);
  }

private:
  ::dt_localization::msg::Dt35Distances msg_;
};

class Init_Dt35Distances_valid
{
public:
  explicit Init_Dt35Distances_valid(::dt_localization::msg::Dt35Distances & msg)
  : msg_(msg)
  {}
  Init_Dt35Distances_frame_count valid(::dt_localization::msg::Dt35Distances::_valid_type arg)
  {
    msg_.valid = std::move(arg);
    return Init_Dt35Distances_frame_count(msg_);
  }

private:
  ::dt_localization::msg::Dt35Distances msg_;
};

class Init_Dt35Distances_distance
{
public:
  explicit Init_Dt35Distances_distance(::dt_localization::msg::Dt35Distances & msg)
  : msg_(msg)
  {}
  Init_Dt35Distances_valid distance(::dt_localization::msg::Dt35Distances::_distance_type arg)
  {
    msg_.distance = std::move(arg);
    return Init_Dt35Distances_valid(msg_);
  }

private:
  ::dt_localization::msg::Dt35Distances msg_;
};

class Init_Dt35Distances_header
{
public:
  Init_Dt35Distances_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Dt35Distances_distance header(::dt_localization::msg::Dt35Distances::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Dt35Distances_distance(msg_);
  }

private:
  ::dt_localization::msg::Dt35Distances msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::dt_localization::msg::Dt35Distances>()
{
  return dt_localization::msg::builder::Init_Dt35Distances_header();
}

}  // namespace dt_localization

#endif  // DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__BUILDER_HPP_
