#pragma once

#include "lib.hpp"
#include "port.hpp"
#include "time.hpp"

namespace Stp {
namespace PerfParams {

inline u16 HelloTime(PortH port) {
    return port.DesignatedTimes().HelloTime();
}

inline u16 MigrateTime() noexcept {
    /// @todo Make it dynamic managementable
    return Time::RecommendedValue::MigrateTime;
}

inline u8 TxHoldCount() noexcept {
    /// @todo Make it dynamic managementable
    return Port::RecommendedValue::TransmitHoldCount;
}

} // namespace PerfParams
} // namespace SpanningTree
