#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to dt_localization__msg__Dt35Distances

// This struct is not documented.
#[allow(missing_docs)]

#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Dt35Distances {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::Header,

    /// 四个测距端口的距离，单位米。
    /// 端口会保留上一次有效读数；`valid` 指示哪些端口在本帧中被更新。
    pub distance: [f32; 4],


    // This member is not documented.
    #[allow(missing_docs)]
    pub valid: [bool; 4],

    /// 自节点启动以来的诊断计数。
    /// 成功解码的帧数
    pub frame_count: u32,

    /// 因 CRC 校验失败被丢弃的帧数
    pub crc_error_count: u32,

}



impl Default for Dt35Distances {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::msg::rmw::Dt35Distances::default())
  }
}

impl rosidl_runtime_rs::Message for Dt35Distances {
  type RmwMsg = super::msg::rmw::Dt35Distances;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Owned(msg.header)).into_owned(),
        distance: msg.distance,
        valid: msg.valid,
        frame_count: msg.frame_count,
        crc_error_count: msg.crc_error_count,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        header: std_msgs::msg::Header::into_rmw_message(std::borrow::Cow::Borrowed(&msg.header)).into_owned(),
        distance: msg.distance,
        valid: msg.valid,
      frame_count: msg.frame_count,
      crc_error_count: msg.crc_error_count,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      header: std_msgs::msg::Header::from_rmw_message(msg.header),
      distance: msg.distance,
      valid: msg.valid,
      frame_count: msg.frame_count,
      crc_error_count: msg.crc_error_count,
    }
  }
}


