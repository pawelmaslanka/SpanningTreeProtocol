/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

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
