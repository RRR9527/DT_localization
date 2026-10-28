std_msgs/Header header

# 四个测距端口的距离，单位毫米（协议原始值，不换算）。
# 毫米 → 米的换算只在 localization 包的集中适配层做一次，其余链路统一用米。
# 端口会保留上一次有效读数；`valid` 指示哪些端口在本帧中被更新。
float32[4] distance
bool[4]    valid

# 自节点启动以来的诊断计数。
uint32 frame_count      # 成功解码的帧数
uint32 crc_error_count  # 因 CRC 校验失败被丢弃的帧数
