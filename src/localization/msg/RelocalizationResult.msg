std_msgs/Header header

# 状态码（与代码中的 SolveStatus 一致）。
uint8 OK = 0
uint8 DEVIATED = 1
uint8 LOW_OBSERVABILITY = 2
uint8 NOT_ENOUGH_SAMPLES = 3
uint8 SOLVE_FAILED = 4
uint8 status

# 估计位姿 q_hat = [x, y, psi]^T（m、m、rad），位于墙平面所在的世界系。
float64 x
float64 y
float64 psi

# q_hat 的 3x3 协方差，行主序，顺序 x / y / psi。
# 求解失败或样本不足时退化为先验协方差 P0。
float64[9] covariance

# Delta = q_hat - q0（m、m、rad）。
float64[3] delta

# 偏离检测：q0 处残差的马氏距离；超过 mahalanobis_threshold 判 DEVIATED。
float64 mahalanobis_distance
# 可观测性：信息矩阵 A 的条件数；超过 cond_warn_threshold 判 LOW_OBSERVABILITY。
float64 condition_number

# 采样统计：每通道剔异常后的样本数 / 采样窗口内收到的帧数 / 每通道均值（m）。
uint32[3] sample_counts
uint32 frame_count
float64[3] z_mean

# Gauss-Newton 实际迭代次数（未进入求解时为 0）。
uint32 iterations
