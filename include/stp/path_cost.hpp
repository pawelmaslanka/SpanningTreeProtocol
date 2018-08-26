/***************************************************************************************************
Copyright (c) 2018, Pawel Maslanka <pawmas@hotmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
***************************************************************************************************/

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
