#pragma once

#include "lib.hpp"
#include "time.hpp"

namespace SpanningTree {

class PerfParams {
public:
    static u16 MigrateTime() noexcept;
};

inline u16 PerfParams::MigrateTime() noexcept {
    /// @todo Make it dynamic managementable
    return Time::RecommendedValue::MigrateTime;
}

} // namespace SpanningTree
