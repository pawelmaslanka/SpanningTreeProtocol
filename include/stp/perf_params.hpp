#pragma once

// This project's headers
#include "lib.hpp"
#include "port.hpp"
#include "time.hpp"

namespace Stp {
namespace PerfParams {

u16 HelloTime(const Port& port) noexcept;
u16 MigrateTime() noexcept;
u8 TxHoldCount() noexcept;

inline u16 HelloTime(const Port& port) noexcept {
    return port.DesignatedTimes().HelloTime();
}

inline u16 MigrateTime() noexcept {
    /// @todo Make it dynamic managementable
    return +Time::RecommendedValue::MigrateTime;
}

inline u8 TxHoldCount() noexcept {
    /// @todo Make it dynamic managementable
    return Port::RecommendedValue::TransmitHoldCount;
}

} // namespace PerfParams
} // namespace Stp
