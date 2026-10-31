std_msgs/Header header

# 四个通道的原始距离，由 get_serial_msg 透传（单位 m）。
# 保留全部通道是为了扩展性：目前只取其中三个通道解算，
# 剩下的通道以后可直接从本话题取用，无需再订阅串口话题。
float32[4] distance
bool[4]    valid

# 参与本次解算的通道下标（对应 Dt35Distances.distance 的下标，0 起始）。
int32 channel_1
int32 channel_2
int32 channel_3

# 解算结果。
float32 cur_d1   # 通道 channel_1 原始距离（m）
float32 cur_d2   # 通道 channel_2 原始距离（m）
float32 cur_d3   # 通道 channel_3 原始距离（m）
float32 ave_d1   # 通道 channel_1 平均距离，已加 bias_ch_1 修正（m）
float32 ave_d2   # 通道 channel_2 平均距离，已加 bias_ch_2 修正（m）
float32 ave_d3   # 通道 channel_3 平均距离，已加 bias_ch_3 修正（m）
float32 d_x  # 到墙面x的距离（m），公式见 localization_node.hpp 头注释
float32 d_y  # 到墙面y的距离（m），公式见 localization_node.hpp 头注释
float32 yaw  # 相对墙面的偏航角（rad），已减 bias_yaw
