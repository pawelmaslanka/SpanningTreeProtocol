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
#include "bpdu.hpp"
#include "bridge_id.hpp"
#include "lib.hpp"
#include "mac.hpp"
#include "path_cost.hpp"
#include "port_id.hpp"

// C++ Standard Library
#include <array>

namespace Stp {

class PriorityVector {
public:
    /// @brief Table 17-2
    /// @todo Make it dynamic configurable
    enum class RecommendedBridgePriority : u16 {
        Value = 32768,
        Step = 4096
    };

    /// @brief Table 17-2
    /// @todo Make it dynamic configurable
    enum class RecommendedPortPriority : u8 {
        Value = 128,
        Step = 16
    };

    /**
     * @brief PriorityVector
     * @param rootBridgeIdData By default, set to value which is considered as the worst assignable value
     * from RSTP point of view.
     * @param rootPathCostData By default, set to value which is considered as the worst assignable value
     * from RSTP point of view.
     * @param dsgBridgeIdData By default, set to value which is considered as the worst assignable value
     * from RSTP point of view.
     * @param dsgPortIdData By default, set to value which is considered as the worst assignable value
     * from RSTP point of view.
     */
    explicit PriorityVector() noexcept = default;
    PriorityVector(const BridgeId& rootBridgeIdData, const PathCost& rootPathCostData,
                   const BridgeId& designatedBridgeId, const PortId& designatedPortId) noexcept;
    PriorityVector(const PriorityVector&) noexcept = default;
    PriorityVector(PriorityVector&&) = default;

    ~PriorityVector() noexcept = default;

    PriorityVector& operator=(const PriorityVector&) noexcept = default;
    PriorityVector& operator=(PriorityVector&&) = default;

    bool operator<(const PriorityVector& comparedTo) const noexcept;
    bool operator==(const PriorityVector& comparedTo) const noexcept;

    const BridgeId& RootBridgeId() const noexcept;
    void SetRootBridgeId(const BridgeId& value) noexcept;

    const PathCost& RootPathCost() const noexcept;
    PathCost& GetRootPathCost() noexcept;
    void SetRootPathCost(PathCost& value) noexcept;

    const BridgeId& DesignatedBridgeId() const noexcept;
    void SetDesignatedBridgeId(const BridgeId&) noexcept;

    const PortId& DesignatedPortId() const noexcept;
    void SetDesignatedPortId(const PortId& value) noexcept;

private:
    BridgeId _rootBridgeId;
    PathCost _rootPathCost;
    BridgeId _bridgeId;
    PortId _portId;
}; // End of 'PriorityVector' class declaration

inline const BridgeId& PriorityVector::RootBridgeId() const noexcept { return _rootBridgeId; }
inline void PriorityVector::SetRootBridgeId(const BridgeId& value) noexcept { _rootBridgeId = value; }

inline const PathCost& PriorityVector::RootPathCost() const noexcept { return _rootPathCost; }
inline PathCost& PriorityVector::GetRootPathCost() noexcept { return _rootPathCost; }
inline void PriorityVector::SetRootPathCost(PathCost& value) noexcept { _rootPathCost = value; }

inline const BridgeId& PriorityVector::DesignatedBridgeId() const noexcept { return _bridgeId; }
inline void PriorityVector::SetDesignatedBridgeId(const BridgeId& value) noexcept { _bridgeId = value; }

inline const PortId& PriorityVector::DesignatedPortId() const noexcept { return _portId; }
inline void PriorityVector::SetDesignatedPortId(const PortId& value) noexcept { _portId = value; }

} // namespace Stp
