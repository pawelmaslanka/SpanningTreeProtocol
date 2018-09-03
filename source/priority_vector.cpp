/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// This project's headers
#include "stp/priority_vector.hpp"

namespace Stp {

PriorityVector::PriorityVector(const BridgeId& rootBridgeId, const PathCost& rootPathCost,
                               const BridgeId& designatedBridgeId, const PortId& designatedPortId) noexcept
    : _rootBridgeId{ rootBridgeId }, _rootPathCost{ rootPathCost }, _bridgeId{ designatedBridgeId },
      _portId{ designatedPortId } {
    // Nothing to do more
}

bool PriorityVector::operator<(const PriorityVector& comparedTo) const noexcept {
    if (_rootBridgeId < comparedTo._rootBridgeId) {
        return true;
    }

    if ((_rootBridgeId == comparedTo._rootBridgeId)
            && (_rootPathCost < comparedTo._rootPathCost)) {
        return true;
    }

    if ((_rootBridgeId == comparedTo._rootBridgeId)
            && (_rootPathCost == comparedTo._rootPathCost)
            && (_bridgeId < comparedTo._bridgeId)) {
        return true;
    }

    if (((_rootBridgeId == comparedTo._rootBridgeId)
         && (_rootPathCost == comparedTo._rootPathCost)
         && (_bridgeId == comparedTo._bridgeId)
         && (_portId < comparedTo._portId))) {
        return true;
    }

    if ((_bridgeId.Address() == comparedTo._bridgeId.Address())
            && (_portId.PortNum() == comparedTo._portId.PortNum())) {
        return true;
    }

    return false;
}

bool PriorityVector::operator==(const PriorityVector& comparedTo) const noexcept {
    return (_rootBridgeId == comparedTo._rootBridgeId) && (_rootPathCost == comparedTo._rootPathCost)
            && (_bridgeId == comparedTo._bridgeId) && (_portId == comparedTo._portId);
}

} // namespace Rstp
