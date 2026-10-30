std_msgs/Header header

# 四个通道的原始距离，由 get_serial_msg 透传（单位 m）。
# 保留全部通道是为了扩展性：目前只取其中两个通道解算，
# 剩下的通道以后可直接从本话题取用，无需再订阅串口话题。
float32[4] distance
bool[4]    valid

# 参与本次解算的通道下标（对应 Dt35Distances.distance 的下标，0 起始）。
int32 channel_1
int32 channel_2
int32 channel_3

# 解算结果。
float32 d1   # 通道 channel_1 距离（m）
float32 d2   # 通道 channel_2 距离（m）
float32 d3   # 通道 channel_3 距离（m）
float32 d_x  # d_x = (d1 + d2) / 2 + len_x，到墙面x的平均距离（m）
float32 d_y  # d_y = \frac{d3 l}{\sqrt{l^2 + (d1 - d2)^2}} + len_y，到墙面y的距离（m）
float32 yaw  # yaw = atan((d1 - d2) / L)，相对墙面的偏航角（rad）
