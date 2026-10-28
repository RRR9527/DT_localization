// 距离数据集中适配层。
//
// 全链路唯一的“毫米 → 米”换算点：get_serial_msg 发布的是协议原始毫米值，
// 本类按 channel_indices 取出 3 个通道、检查有效位并一次性换算成米；
// 之后的采样、模型、求解、发布全部使用米。
//
// channel_indices[j] 是 Dt35Distances.distance 的下标（0 起始），
// 与配置里的 dt{j+1} 一一对应（j = 0, 1, 2）。

#ifndef LOCALIZATION__DISTANCE_ADAPTER_HPP_
#define LOCALIZATION__DISTANCE_ADAPTER_HPP_

#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>

#include "get_serial_msg/msg/dt35_distances.hpp"

namespace localization
{

// 一帧消息换算后的 3 通道读数：距离（米）与各自的有效标志。
struct Reading
{
    std::array<double, 3> distance_m{{0.0, 0.0, 0.0}};
    std::array<bool, 3> valid{{false, false, false}};
};

class DistanceAdapter
{
public:
    // indices 必须是 3 个互不相同的 [0, 3] 内下标。
    explicit DistanceAdapter(const std::vector<int64_t> & indices);

    // 取出 3 个通道的原始毫米值，检查 valid 位，换算成米（全链路只在这里做一次）。
    Reading Convert(const get_serial_msg::msg::Dt35Distances & msg) const;

private:
    std::array<size_t, 3> indices_{{0U, 0U, 0U}};
};

}  // namespace localization

#endif  // LOCALIZATION__DISTANCE_ADAPTER_HPP_
