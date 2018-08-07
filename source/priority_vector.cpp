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

#include "priority_vector.hpp"

namespace SpanningTree {

PriorityVector::PriorityVector(const BridgeId& rootBridgeId, const PathCost& rootPathCost,
                               const BridgeId& designatedBridgeId, const PortId& designatedPortId) noexcept
    : _rootBridgeId{ rootBridgeId }, _rootPathCost{ rootPathCost }, _bridgeId{ designatedBridgeId },
      _portId{ designatedPortId }
{
    // Nothing to do more
}

bool PriorityVector::operator<(const PriorityVector& comparedTo) const noexcept
{
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

bool PriorityVector::operator==(const PriorityVector& comparedTo) const noexcept
{
    return (_rootBridgeId == comparedTo._rootBridgeId) && (_rootPathCost == comparedTo._rootPathCost)
            && (_bridgeId == comparedTo._bridgeId) && (_portId == comparedTo._portId);
}

} // namespace Rstp
