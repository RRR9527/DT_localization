// 三测距仪 + 双墙平面定位模型与带先验的 Gauss-Newton 求解器。
//
// 模型（车体视为长方体，[x,y] 为车体参考点在水平面上的位置，psi 为偏航角）：
//   Rz(psi) = [[cos,-sin,0],[sin,cos,0],[0,0,1]]
//   p_i(q)  = [x,y,0]^T + Rz(psi) s_i      —— 第 i 个 DT 安装点在世界系的位置
//   w_i(q)  = Rz(psi) u_i                  —— 光轴方向（世界系）
//   d_i(q)  = k_i (c_i - n_i^T p_i) / (n_i^T w_i) + b_i
// |n_i^T w_i| < denom_min_abs 时该路测量判无效，从当次求解中剔除。
// 两面墙只要求各自给出平面方程，允许不严格正交。
//
// 求解（MAP 意义下等价于带先验的加权最小二乘）：
//   Sigma = diag(sigma_d_i^2 + sigma_sys_i^2)
//   J = ∂h/∂q（中心差分数值雅可比，eps = gn_diff_eps）
//   A = J^T Sigma^-1 J + P0^-1
//   b = J^T Sigma^-1 (z - h(q)) + P0^-1 (q0 - q)
//   dq = A.ldlt().solve(b); q += dq
//   最多 gn_max_iter 次，||dq|| < gn_step_tol 停。
//   收敛后 q_hat = q，P = (J^T Sigma^-1 J + P0^-1)^-1。
//
// 偏离检测：q0 处 r = z - h(q0)，S = Sigma + J0 P0 J0^T，
//   马氏距离 m = r^T S^-1 r > mahalanobis_threshold 判 DEVIATED。
// 可观测性：A 的条件数（SVD 奇异值之比）> cond_warn_threshold 判 LOW_OBSERVABILITY。
// 初值用先验 q0（如需几何公式初值可在此扩展，最终结果以 GN 为准）。

#ifndef LOCALIZATION__RELOCALIZER_HPP_
#define LOCALIZATION__RELOCALIZER_HPP_

#include <array>
#include <cstddef>
#include <cstdint>

#include <Eigen/Dense>

namespace localization
{

// 单个 DT 的标定与噪声参数（长度单位全部为米）。
struct SensorConfig
{
    Eigen::Vector3d s{Eigen::Vector3d::Zero()};    // 安装点 s=[a,b,h]^T，车体系（m）
    Eigen::Vector3d u{Eigen::Vector3d::UnitX()};   // 光轴单位向量，车体系（加载时会归一化）
    Eigen::Vector3d n{Eigen::Vector3d::UnitX()};   // 墙平面单位法线，世界系（加载时会归一化）
    double c = 0.0;        // 墙平面 n^T X = c（m）
    double b = 0.0;        // 距离偏置（m）
    double k = 1.0;        // 距离比例因子（无量纲）
    double sigma_d = 0.01; // 测量噪声标准差（m）
    double sigma_sys = 0.02; // 系统误差地板（m）
};

// 求解器可调参数（默认值即需求给定值）。
struct SolverParams
{
    double denom_min_abs = 1.0e-6;        // |n^T w| 下限，过小判该路测量无效
    double gn_diff_eps = 1.0e-6;          // 数值雅可比中心差分步长
    double gn_step_tol = 1.0e-8;          // 收敛判据 ||dq||
    uint32_t gn_max_iter = 20U;           // 最大迭代次数
    double yaw_max = 1.0;                 // |psi_hat| 超限视为求解失败（rad）
    double cond_warn_threshold = 1000.0;  // 信息矩阵 A 条件数告警阈值
    double mahalanobis_threshold = 11.34; // 马氏距离阈值（chi2_3_0.99）
};

// 状态码，与 RelocalizationResult.msg 的常量保持一致。
enum class SolveStatus : uint8_t
{
    kOk = 0,
    kDeviated = 1,
    kLowObservability = 2,
    kNotEnoughSamples = 3,
    kSolveFailed = 4,
};

struct SolveResult
{
    SolveStatus status = SolveStatus::kOk;
    Eigen::Vector3d q_hat{Eigen::Vector3d::Zero()}; // [x, y, psi]（m, m, rad）
    Eigen::Matrix3d P{Eigen::Matrix3d::Zero()};     // 后验协方差（失败时退化为 P0）
    Eigen::Vector3d delta{Eigen::Vector3d::Zero()}; // q_hat - q0
    double mahalanobis = 0.0;        // q0 处残差马氏距离
    bool mahalanobis_valid = true;   // q0 处是否有有效测量可供偏离检测
    double condition_number = 0.0;   // 信息矩阵 A 的条件数
    uint32_t iterations = 0U;        // 实际迭代次数
};

class Relocalizer
{
public:
    explicit Relocalizer(const SolverParams & params);

    void setParams(const SolverParams & params);

    // z_mean：3 通道剔异常后的均值（米）；q0 / P0：先验均值与协方差。
    SolveResult Solve(const std::array<SensorConfig, 3> & sensors,
                      const Eigen::Vector3d & z_mean,
                      const Eigen::Vector3d & q0,
                      const Eigen::Matrix3d & P0) const;

private:
    // 第 i 路模型值；|n^T w| 过小返回 false。
    bool Predict(size_t i, const SensorConfig & sensor, const Eigen::Vector3d & q,
                 double * distance) const;

    // 数值雅可比（中心差分）；无效行整行置 0，并清除对应 valid 标志。
    Eigen::Matrix3d Jacobian(const std::array<SensorConfig, 3> & sensors,
                             const Eigen::Vector3d & q,
                             std::array<bool, 3> * valid) const;

    SolverParams params_;
};

}  // namespace localization

#endif  // LOCALIZATION__RELOCALIZER_HPP_
