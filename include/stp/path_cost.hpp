/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "lib.hpp"

namespace Stp {

class PathCost {
public:
    static u32 SpeedMbToPathCostValue(const u32 speedMb) noexcept;

    /**
     * @brief PathCost
     * @param speedMb By default, set to value which is considered as the worst assignable value
     * from RSTP point of view.
     */
    explicit PathCost(const u32 speedMb = 0) noexcept;
    PathCost(const PathCost&) noexcept = default;
    PathCost(PathCost&&) = default;

    ~PathCost() noexcept = default;

    void operator+=(const PathCost& pathCost) noexcept;
    PathCost& operator=(const PathCost&) noexcept = default;
    PathCost& operator=(PathCost&&) = default;

    bool operator==(const PathCost& comparedTo) const noexcept;
    bool operator<(const PathCost& comparedTo) const noexcept;

    u32 Value() const noexcept;
    u32 ConvertToBpduData() const noexcept;

    void SetPathCost(const u32 pathCost) noexcept;

private:
    union BpduData {
        u8 stream[sizeof (u32)];
        struct {
            u32 MostSignificant1st: 8;
            u32 MostSignificant2nd: 8;
            u32 MostSignificant3rd: 8;
            u32 MostSignificant4th: 8;
        } Fields;
    };
    /// @brief Stores value of path cost component.
    u32 _value;
};

inline void PathCost::operator+=(const PathCost& pathCost) noexcept {
    _value += pathCost._value;
}

inline bool PathCost::operator==(const PathCost& comparedTo) const noexcept {
    return _value == comparedTo._value;
}

inline bool PathCost::operator<(const PathCost& comparedTo) const noexcept {
    return _value > comparedTo._value;
}

inline u32 PathCost::Value() const noexcept { return _value; }

inline void PathCost::SetPathCost(const u32 pathCost) noexcept { _value = pathCost; }

} // namespace Rstp
