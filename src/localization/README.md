# localization —— 三测距仪 + 双墙平面定位与偏离检测

车体（长方体，体积不可忽略）安装 3 个 DT 测距仪，借助两面（可能不严格正交的）墙，
静止采样一段时间后估计初始平面位姿 `q = [x, y, psi]^T`，并检测当前测量相对先验位姿的偏离。

## 架构与数据流

```
get_serial_msg 节点          localization 节点
┌──────────────┐   Dt35Distances(mm)   ┌─────────────────────────────────────┐
│ 串口解包      │ ────────────────────▶ │ DistanceAdapter 集中适配层           │
│ 发布原始毫米值 │                      │  · 按 channel_indices 选 3 个通道    │
└──────────────┘                      │  · 毫米 → 米（全链路唯一换算点）      │
                                      │            ↓                        │
                                      │ 采样(3s) → 中位数+MAD 剔异常 → z_mean │
                                      │            ↓                        │
                                      │ Relocalizer：模型 + GN 求解器        │
                                      │  · q_hat、P、Delta、马氏距离、条件数   │
                                      │            ↓                        │
                                      │ 发布 ~/result（自定义消息）           │
                                      │ 发布 ~/pose（PoseWithCovarianceStamped）│
                                      └─────────────────────────────────────┘
```

**单位约定**：`get_serial_msg` 发布协议原始毫米值；毫米 → 米的换算**只在
`DistanceAdapter` 做一次**，之后采样、模型、求解、发布全链路使用米。

## 模型

```
Rz(psi) = [[cos,-sin,0],[sin,cos,0],[0,0,1]]
p_i(q)  = [x,y,0]^T + Rz(psi) s_i        # 第 i 个 DT 安装点（世界系），s_i=[a,b,h] 车体系
w_i(q)  = Rz(psi) u_i                    # 光轴方向（世界系）
d_i(q)  = k_i (c_i - n_i^T p_i) / (n_i^T w_i) + b_i
```

- `n_i^T X = c_i` 是第 i 路对应的墙平面方程（`n_i` 单位法线），两面墙允许不严格正交；
- `|n_i^T w_i(q)| < denom_min_abs` 时该路测量判无效，从当次求解中剔除；
- 观测模型 `z = [d1,d2,d3]^T = h(q) + eps`，`eps ~ N(0, Sigma)`，
  `Sigma` 对角，`sigma_i^2 = sigma_d_i^2 + sigma_sys_i^2`。

## 采样与求解

1. 启动后（或 `~/resample` 服务触发后）静止采样 `sample_duration` 秒；
2. 每通道：中位数 + MAD 剔异常（`|x - med| <= 3 * 1.4826 * MAD`）后求均值 `z_mean`；
3. 任一通道内点数 < `min_samples` → 状态 `NOT_ENOUGH_SAMPLES`，不求解；
4. 带先验 `q0 / P0` 的 Gauss-Newton（初值 = 先验 `q0`，最终结果以 GN 为准）：

```
J = ∂h/∂q（中心差分数值雅可比，eps = gn_diff_eps）
A = J^T Sigma^-1 J + P0^-1
b = J^T Sigma^-1 (z_mean - h(q)) + P0^-1 (q0 - q)
dq = A.ldlt().solve(b); q += dq      # 最多 gn_max_iter 次，||dq|| < gn_step_tol 停
```

5. 收敛后输出 `q_hat` 与后验协方差 `P = (J^T Sigma^-1 J + P0^-1)^-1`。

## 偏离检测与可观测性

- **偏离**：在 `q0` 处取残差 `r = z_mean - h(q0)`，`S = Sigma + J0 P0 J0^T`，
  马氏距离 `m = r^T S^-1 r`；`m > mahalanobis_threshold`（默认 chi2(3,0.99) ≈ 11.34）
  标记 **DEVIATED**。
- **可观测性**：对信息矩阵 `A` 求条件数（SVD 奇异值之比），
  超过 `cond_warn_threshold` 标记 **LOW_OBSERVABILITY**（结果仍照常输出）。
- **求解失败**：三路测量全部几何无效、迭代不收敛、或 `|psi_hat| > yaw_max`
  （偏航角超出物理合理范围）标记 **SOLVE_FAILED**。

状态优先级：`SOLVE_FAILED > DEVIATED > LOW_OBSERVABILITY > OK`
（`NOT_ENOUGH_SAMPLES` 在求解前单独判定）。同时触发多个条件时按该优先级报告。

## 状态码

| 值 | 状态 | 含义 |
|---|---|---|
| 0 | `OK` | 求解收敛且各项检查通过 |
| 1 | `DEVIATED` | 测量相对先验 q0 的马氏距离超阈值 |
| 2 | `LOW_OBSERVABILITY` | 信息矩阵条件数超阈值（仍输出结果） |
| 3 | `NOT_ENOUGH_SAMPLES` | 至少一个通道样本数不足 |
| 4 | `SOLVE_FAILED` | 几何无效 / 不收敛 / yaw 超限 |

## 配置（唯一权威来源：`config/dt_relocalizer.yaml`）

节点通过 ROS 2 参数读取，参数由 launch 从 `config/dt_relocalizer.yaml` 加载；
**所有标定与配置以 YAML 为准**，README 此处只做摘要（单位见 YAML 内注释）。

| 参数 | 默认值 | 说明 |
|---|---|---|
| `distance_topic` | `/get_serial_msg/distances` | 订阅话题 |
| `channel_indices` | `[0, 1, 2]` | 3 个通道下标，第 j 个对应 `dt{j+1}` |
| `frame_id` | `map` | 结果 / 位姿消息 frame_id |
| `sample_duration` | `3.0` | 静止采样时长（s） |
| `min_samples` | `50` | 每通道最少内点数 |
| `q0_x / q0_y / q0_psi` | `0 / 0 / 0` | 先验均值 |
| `sigma_x0 / sigma_y0 / sigma_psi0` | `0.5 / 0.5 / 0.35` | 先验标准差（P0 对角取平方） |
| `gn_max_iter / gn_step_tol / gn_diff_eps` | `20 / 1e-8 / 1e-6` | GN 迭代参数 |
| `denom_min_abs` | `1e-6` | 分母有效性下限 |
| `yaw_max` | `1.0` | psi 解合理性上限（rad） |
| `cond_warn_threshold` | `1000.0` | 条件数告警阈值 |
| `mahalanobis_threshold` | `11.34` | 马氏距离阈值 |
| `dt{i}.s` | — | 安装点 `[a,b,h]`（m，车体系） |
| `dt{i}.u` | — | 光轴单位向量（车体系，自动归一化） |
| `dt{i}.n` | — | 墙平面单位法线（世界系，自动归一化） |
| `dt{i}.c / b / k` | — | 墙平面常数（m）/ 距离偏置（m）/ 比例因子 |
| `dt{i}.sigma_d / sigma_sys` | — | 测量噪声 / 系统误差地板（m） |

## 接口

- **话题**
  - `~/result`（`localization/msg/RelocalizationResult`）：`status`、`q_hat=[x,y,psi]`、
    3x3 协方差（行主序，x/y/psi）、`delta`、`mahalanobis_distance`、`condition_number`、
    `sample_counts`、`frame_count`、`z_mean`、`iterations`；
  - `~/pose`（`geometry_msgs/PoseWithCovarianceStamped`）：同样的位姿与协方差
    （6x6 只填 x/y/yaw），方便 rviz 直接可视化。
- **服务**：`~/resample`（`std_srvs/srv/Trigger`）——随时触发重新采样 + 求解。

## 使用方法

```bash
# 1. 串口节点（真机）
ros2 launch get_serial_msg get_serial_msg.launch.py port:=/dev/ttyUSB0

# 2. 定位节点（默认加载 config/dt_relocalizer.yaml）
ros2 launch localization localization.launch.py
# 换配置 / 改部分参数：
ros2 launch localization localization.launch.py config:=/path/to/other.yaml
ros2 run localization localization_node --ros-args \
  -p sample_duration:=1.0 -p q0_x:=0.5

# 3. 查看结果（约 sample_duration 秒后发布）
ros2 topic echo /localization_node/result --once
ros2 topic echo /localization_node/pose --once

# 4. 触发重新采样求解
ros2 service call /localization_node/resample std_srvs/srv/Trigger {}
```

**注意**：采样期间车体必须静止；采样结束后结果发布一次，之后按需用服务重触发。

## 无真机测试

用 `../temp/serial_frame_sender.py`（pty 模拟 MCU）跑通全链路：

```bash
# 终端 A：模拟器（打印 slave port，如 /dev/pts/5）
python3 ../temp/serial_frame_sender.py

# 终端 B：串口节点 + 定位节点（source 工作空间后）
ros2 launch get_serial_msg get_serial_msg.launch.py port:=/dev/pts/5
# 终端 C（等待 sample_duration 后）
ros2 topic echo /localization_node/result --once
```

演示配置（YAML 默认值）对应的真值场景：q=[0,0,0] 时
`d1=d2=2.0 m`（墙 x=2）、`d3=1.5 m`（墙 y=1.5）。
