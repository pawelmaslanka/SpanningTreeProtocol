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

#include "stp/bridge.hpp"

// C++ Standard Library
#include <utility>

namespace Stp {

Bridge::Bridge(SystemH system) noexcept
    : _begin{ false }, _bridgeId{ },
      _bridgePriority{ },
      _bridgeTimes{ }, _rootPortId{ },
      _rootPriority{ },
      _rootTimes{ }, _addr{ },
      _system{ system } {
    _bridgeId.SetPriority(+PriorityVector::RecommendedBridgePriority::Value);
    _bridgeId.SetExtension(Bridge::ExtensionDefaultValue);

    _bridgePriority.GetRootPathCost().SetPathCost(0);
    _bridgePriority.SetDesignatedBridgeId(_bridgeId);
    _bridgePriority.SetDesignatedPortId(PortId());
}

void Bridge::AddPort(const u16 portNo) {
    if (_ports.find(portNo) != _ports.end()) {
        return;
    }

    _ports.emplace(std::make_pair(portNo, std::make_shared<Port>()));
}

void Bridge::RemovePort(const u16 portNo) {
    _ports.erase(portNo);
}

PortH Bridge::GetPort(const u16 portNo) {
    auto findIt { _ports.find(portNo) };
    if (_ports.end() == findIt) {
        return PortH{};
    }

    return findIt->second;
}

std::map<u16, PortH>& Bridge::GetAllPorts() {
    return _ports;
}

} // namespace Rstp
