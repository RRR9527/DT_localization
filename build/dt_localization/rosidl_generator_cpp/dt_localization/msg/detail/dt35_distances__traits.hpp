// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from dt_localization:msg/Dt35Distances.idl
// generated code does not contain a copyright notice

#ifndef DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__TRAITS_HPP_
#define DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "dt_localization/msg/detail/dt35_distances__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace dt_localization
{

namespace msg
{

inline void to_flow_style_yaml(
  const Dt35Distances & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: distance
  {
    if (msg.distance.size() == 0) {
      out << "distance: []";
    } else {
      out << "distance: [";
      size_t pending_items = msg.distance.size();
      for (auto item : msg.distance) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: valid
  {
    if (msg.valid.size() == 0) {
      out << "valid: []";
    } else {
      out << "valid: [";
      size_t pending_items = msg.valid.size();
      for (auto item : msg.valid) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: frame_count
  {
    out << "frame_count: ";
    rosidl_generator_traits::value_to_yaml(msg.frame_count, out);
    out << ", ";
  }

  // member: crc_error_count
  {
    out << "crc_error_count: ";
    rosidl_generator_traits::value_to_yaml(msg.crc_error_count, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Dt35Distances & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: distance
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.distance.size() == 0) {
      out << "distance: []\n";
    } else {
      out << "distance:\n";
      for (auto item : msg.distance) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: valid
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.valid.size() == 0) {
      out << "valid: []\n";
    } else {
      out << "valid:\n";
      for (auto item : msg.valid) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: frame_count
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "frame_count: ";
    rosidl_generator_traits::value_to_yaml(msg.frame_count, out);
    out << "\n";
  }

  // member: crc_error_count
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "crc_error_count: ";
    rosidl_generator_traits::value_to_yaml(msg.crc_error_count, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Dt35Distances & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace dt_localization

namespace rosidl_generator_traits
{

[[deprecated("use dt_localization::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const dt_localization::msg::Dt35Distances & msg,
  std::ostream & out, size_t indentation = 0)
{
  dt_localization::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use dt_localization::msg::to_yaml() instead")]]
inline std::string to_yaml(const dt_localization::msg::Dt35Distances & msg)
{
  return dt_localization::msg::to_yaml(msg);
}

template<>
inline const char * data_type<dt_localization::msg::Dt35Distances>()
{
  return "dt_localization::msg::Dt35Distances";
}

template<>
inline const char * name<dt_localization::msg::Dt35Distances>()
{
  return "dt_localization/msg/Dt35Distances";
}

template<>
struct has_fixed_size<dt_localization::msg::Dt35Distances>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<dt_localization::msg::Dt35Distances>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<dt_localization::msg::Dt35Distances>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // DT_LOCALIZATION__MSG__DETAIL__DT35_DISTANCES__TRAITS_HPP_
