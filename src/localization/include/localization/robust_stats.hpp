// 稳健统计工具（头文件实现）。
//
// 采样窗口结束后，每个通道用「中位数 + MAD」剔除异常后求均值：
//   med     = median(x)
//   MAD     = median(|x - med|)
//   内点判据 |x - med| <= 3 * 1.4826 * MAD（1.4826 把 MAD 换算成近似标准差）
//   z_mean  = mean(内点)
// 对偶发跳变 / 毛刺不敏感，符合“静止采样”的假设。

#ifndef LOCALIZATION__ROBUST_STATS_HPP_
#define LOCALIZATION__ROBUST_STATS_HPP_

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <utility>
#include <vector>

namespace localization
{

// 中位数（拷贝后排序，不改动调用方数据）。
inline double Median(std::vector<double> values)
{
    if (values.empty())
    {
        return 0.0;
    }
    std::sort(values.begin(), values.end());
    const size_t n = values.size();
    if ((n % 2U) == 1U)
    {
        return values[n / 2U];
    }
    return 0.5 * (values[n / 2U - 1U] + values[n / 2U]);
}

// 中位数 + MAD 剔异常后求均值；inlier_count 输出内点个数。
inline double RobustMean(const std::vector<double> & samples, size_t * inlier_count)
{
    *inlier_count = 0U;
    if (samples.empty())
    {
        return 0.0;
    }

    const double med = Median(samples);

    std::vector<double> deviations;
    deviations.reserve(samples.size());
    for (double x : samples)
    {
        deviations.push_back(std::fabs(x - med));
    }
    const double mad = Median(std::move(deviations));

    // 阈值 = 3 * 1.4826 * MAD ≈ 3σ；MAD 为 0（样本全相同）时全部视为内点。
    const double threshold = 3.0 * 1.4826 * mad;

    double sum = 0.0;
    size_t count = 0U;
    for (double x : samples)
    {
        if (mad <= 0.0 || std::fabs(x - med) <= threshold)
        {
            sum += x;
            count++;
        }
    }
    *inlier_count = count;
    return sum / static_cast<double>(count);
}

}  // namespace localization

#endif  // LOCALIZATION__ROBUST_STATS_HPP_
