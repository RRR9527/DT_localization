#include "localization/distance_adapter.hpp"

#include <stdexcept>

namespace localization
{

namespace
{

// 毫米 → 米。全包唯一的单位换算函数，避免换算散落各处。
constexpr double kMillimeterToMeter = 0.001;

}  // namespace

DistanceAdapter::DistanceAdapter(const std::vector<int64_t> & indices)
{
    if (indices.size() != 3U)
    {
        throw std::invalid_argument("channel_indices must contain exactly 3 indices");
    }
    for (size_t j = 0U; j < 3U; ++j)
    {
        const int64_t idx = indices[j];
        if (idx < 0 || idx >= 4)
        {
            throw std::invalid_argument("channel_indices entries must be in [0, 3]");
        }
        indices_[j] = static_cast<size_t>(idx);
    }
    if (indices_[0] == indices_[1] || indices_[0] == indices_[2] ||
        indices_[1] == indices_[2])
    {
        throw std::invalid_argument("channel_indices entries must be distinct");
    }
}

Reading DistanceAdapter::Convert(const get_serial_msg::msg::Dt35Distances & msg) const
{
    Reading reading;
    for (size_t j = 0U; j < 3U; ++j)
    {
        const size_t idx = indices_[j];
        if (idx >= msg.distance.size() || idx >= msg.valid.size())
        {
            // 消息字段不足（正常不会发生），按无效处理。
            reading.valid[j] = false;
            continue;
        }
        if (msg.valid[idx])
        {
            // 毫米 → 米：全链路唯一的换算点。
            reading.distance_m[j] =
                static_cast<double>(msg.distance[idx]) * kMillimeterToMeter;
            reading.valid[j] = true;
        }
    }
    return reading;
}

}  // namespace localization
