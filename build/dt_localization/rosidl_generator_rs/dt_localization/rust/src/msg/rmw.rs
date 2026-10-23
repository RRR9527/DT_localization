#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "dt_localization__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__dt_localization__msg__Dt35Distances() -> *const std::ffi::c_void;
}

#[link(name = "dt_localization__rosidl_generator_c")]
extern "C" {
    fn dt_localization__msg__Dt35Distances__init(msg: *mut Dt35Distances) -> bool;
    fn dt_localization__msg__Dt35Distances__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<Dt35Distances>, size: usize) -> bool;
    fn dt_localization__msg__Dt35Distances__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<Dt35Distances>);
    fn dt_localization__msg__Dt35Distances__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<Dt35Distances>, out_seq: *mut rosidl_runtime_rs::Sequence<Dt35Distances>) -> bool;
}

// Corresponds to dt_localization__msg__Dt35Distances
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct Dt35Distances {

    // This member is not documented.
    #[allow(missing_docs)]
    pub header: std_msgs::msg::rmw::Header,

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
    unsafe {
      let mut msg = std::mem::zeroed();
      if !dt_localization__msg__Dt35Distances__init(&mut msg as *mut _) {
        panic!("Call to dt_localization__msg__Dt35Distances__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for Dt35Distances {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { dt_localization__msg__Dt35Distances__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { dt_localization__msg__Dt35Distances__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { dt_localization__msg__Dt35Distances__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for Dt35Distances {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for Dt35Distances where Self: Sized {
  const TYPE_NAME: &'static str = "dt_localization/msg/Dt35Distances";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__dt_localization__msg__Dt35Distances() }
  }
}


