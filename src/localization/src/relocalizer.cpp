#include "localization/relocalizer.hpp"

#include <cmath>
#include <limits>
#include <utility>
#include <vector>

#include <Eigen/SVD>

namespace localization
{

namespace
{

// 绕 z 轴旋转矩阵 Rz(psi)。
Eigen::Matrix3d Rz(double psi)
{
    const double c = std::cos(psi);
    const double s = std::sin(psi);
    Eigen::Matrix3d R;
    R << c, -s, 0.0,
         s,  c, 0.0,
         0.0, 0.0, 1.0;
    return R;
}

}  // namespace

Relocalizer::Relocalizer(const SolverParams & params)
: params_(params)
{
}

void Relocalizer::setParams(const SolverParams & params)
{
    params_ = params;
}

bool Relocalizer::Predict(size_t i, const SensorConfig & sensor, const Eigen::Vector3d & q,
                          double * distance) const
{
    (void)i;
    const Eigen::Matrix3d R = Rz(q.z());
    // 车体参考点在水平面 [x, y, 0]，传感器安装点带离地高度 h。
    const Eigen::Vector3d p = Eigen::Vector3d(q.x(), q.y(), 0.0) + R * sensor.s;
    const Eigen::Vector3d w = R * sensor.u;
    const double denom = sensor.n.dot(w);
    if (std::fabs(denom) < params_.denom_min_abs)
    {
        return false;  // 光轴与墙面近乎平行，该路测量几何上不可用。
    }
    *distance = sensor.k * (sensor.c - sensor.n.dot(p)) / denom + sensor.b;
    return true;
}

Eigen::Matrix3d Relocalizer::Jacobian(const std::array<SensorConfig, 3> & sensors,
                                      const Eigen::Vector3d & q,
                                      std::array<bool, 3> * valid) const
{
    Eigen::Matrix3d J = Eigen::Matrix3d::Zero();
    for (size_t i = 0U; i < 3U; ++i)
    {
        if (!(*valid)[i])
        {
            continue;
        }
        double d0 = 0.0;
        if (!Predict(i, sensors[i], q, &d0))
        {
            (*valid)[i] = false;
            continue;
        }
        for (int col = 0; col < 3; ++col)
        {
            Eigen::Vector3d qp = q;
            Eigen::Vector3d qm = q;
            qp[col] += params_.gn_diff_eps;
            qm[col] -= params_.gn_diff_eps;
            double dp = 0.0;
            double dm = 0.0;
            if (!Predict(i, sensors[i], qp, &dp) || !Predict(i, sensors[i], qm, &dm))
            {
                // 差分扰动点越过有效性边界，该路测量整行剔除。
                (*valid)[i] = false;
                J.row(static_cast<Eigen::Index>(i)).setZero();
                break;
            }
            J(static_cast<Eigen::Index>(i), col) =
                (dp - dm) / (2.0 * params_.gn_diff_eps);
        }
    }
    return J;
}

SolveResult Relocalizer::Solve(const std::array<SensorConfig, 3> & sensors,
                               const Eigen::Vector3d & z_mean,
                               const Eigen::Vector3d & q0,
                               const Eigen::Matrix3d & P0) const
{
    SolveResult result;

    const Eigen::Matrix3d P0_inv = P0.inverse();

    // 在给定 q 处组装 A、b，并返回模型值 h 与有效性掩码。
    const auto assemble = [&](const Eigen::Vector3d & qq,
                              Eigen::Vector3d * h_out,
                              std::array<bool, 3> * valid_out) {
        std::array<bool, 3> v{{true, true, true}};
        Eigen::Vector3d hh = Eigen::Vector3d::Zero();
        for (size_t i = 0U; i < 3U; ++i)
        {
            v[i] = Predict(i, sensors[i], qq, &hh[static_cast<Eigen::Index>(i)]);
        }
        const Eigen::Matrix3d JJ = Jacobian(sensors, qq, &v);

        // 先验项 + 有效测量的加权残差项。
        Eigen::Matrix3d AA = P0_inv;
        Eigen::Vector3d bb = P0_inv * (q0 - qq);
        for (size_t i = 0U; i < 3U; ++i)
        {
            if (!v[i])
            {
                continue;
            }
            const double sigma2 = sensors[i].sigma_d * sensors[i].sigma_d +
                                  sensors[i].sigma_sys * sensors[i].sigma_sys;
            const double inv_sigma2 = 1.0 / sigma2;
            const Eigen::Vector3d jrow =
                JJ.row(static_cast<Eigen::Index>(i)).transpose();
            AA += inv_sigma2 * (jrow * jrow.transpose());
            bb += inv_sigma2 * (z_mean[static_cast<Eigen::Index>(i)] -
                                hh[static_cast<Eigen::Index>(i)]) * jrow;
        }
        if (h_out != nullptr)
        {
            *h_out = hh;
        }
        if (valid_out != nullptr)
        {
            *valid_out = v;
        }
        return std::make_pair(AA, bb);
    };

    // ---- Gauss-Newton 迭代（初值 = 先验 q0）----
    Eigen::Vector3d q = q0;
    bool converged = false;
    uint32_t iter = 0U;
    for (iter = 0U; iter < params_.gn_max_iter; ++iter)
    {
        Eigen::Vector3d h = Eigen::Vector3d::Zero();
        std::array<bool, 3> valid{{true, true, true}};
        const auto system = assemble(q, &h, &valid);

        bool any_valid = false;
        for (bool v : valid)
        {
            any_valid = any_valid || v;
        }
        if (!any_valid)
        {
            // 三路测量在当前 q 处全部几何无效，无从求解。
            result.status = SolveStatus::kSolveFailed;
            result.q_hat = q;
            result.P = P0;
            return result;
        }

        const Eigen::Vector3d dq = system.first.ldlt().solve(system.second);
        q += dq;
        if (dq.norm() < params_.gn_step_tol)
        {
            converged = true;
            iter++;
            break;
        }
    }

    if (!converged)
    {
        result.status = SolveStatus::kSolveFailed;
        result.q_hat = q;
        result.P = P0;
        return result;
    }

    result.q_hat = q;
    result.iterations = iter;

    // ---- 收敛点信息矩阵 A、后验协方差 P 与条件数 ----
    Eigen::Vector3d h = Eigen::Vector3d::Zero();
    std::array<bool, 3> valid{{true, true, true}};
    const auto final_system = assemble(q, &h, &valid);
    result.P = final_system.first.inverse();

    const Eigen::JacobiSVD<Eigen::Matrix3d> svd(final_system.first);
    const Eigen::Vector3d singular_values = svd.singularValues();
    if (singular_values.minCoeff() > 0.0)
    {
        result.condition_number =
            singular_values.maxCoeff() / singular_values.minCoeff();
    }
    else
    {
        result.condition_number = std::numeric_limits<double>::infinity();
    }

    // ---- 偏离检测：q0 处残差的马氏距离 m = r^T S^-1 r，S = Sigma + J0 P0 J0^T ----
    Eigen::Vector3d h0 = Eigen::Vector3d::Zero();
    std::array<bool, 3> valid0{{true, true, true}};
    assemble(q0, &h0, &valid0);

    std::vector<Eigen::Index> idx;
    for (size_t i = 0U; i < 3U; ++i)
    {
        if (valid0[i])
        {
            idx.push_back(static_cast<Eigen::Index>(i));
        }
    }
    const Eigen::Index k = static_cast<Eigen::Index>(idx.size());
    result.mahalanobis_valid = (k > 0);
    if (result.mahalanobis_valid)
    {
        const Eigen::Matrix3d J0 = Jacobian(sensors, q0, &valid0);
        Eigen::VectorXd r(k);
        Eigen::MatrixXd Jv(k, 3);
        Eigen::MatrixXd Sv = Eigen::MatrixXd::Zero(k, k);
        for (Eigen::Index j = 0; j < k; ++j)
        {
            const size_t i = static_cast<size_t>(idx[static_cast<size_t>(j)]);
            const double sigma2 = sensors[i].sigma_d * sensors[i].sigma_d +
                                  sensors[i].sigma_sys * sensors[i].sigma_sys;
            r[j] = z_mean[idx[static_cast<size_t>(j)]] -
                   h0[idx[static_cast<size_t>(j)]];
            Jv.row(j) = J0.row(idx[static_cast<size_t>(j)]);
            Sv(j, j) = sigma2;
        }
        Sv += Jv * P0 * Jv.transpose();
        result.mahalanobis = (r.transpose() * Sv.ldlt().solve(r)).value();
    }

    // ---- 状态判定（优先级：求解失败 > 偏离 > 低可观测性 > 正常）----
    result.delta = result.q_hat - q0;
    if (std::fabs(result.q_hat.z()) > params_.yaw_max)
    {
        // 解出的偏航角超出物理合理范围。
        result.status = SolveStatus::kSolveFailed;
    }
    else if (result.mahalanobis_valid &&
             result.mahalanobis > params_.mahalanobis_threshold)
    {
        result.status = SolveStatus::kDeviated;
    }
    else if (result.condition_number > params_.cond_warn_threshold)
    {
        result.status = SolveStatus::kLowObservability;
    }
    else
    {
        result.status = SolveStatus::kOk;
    }
    return result;
}

}  // namespace localization
