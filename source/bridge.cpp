/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// This project's headers
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
      _system{ system },
      _systemLoggingManager { system->Logger } {
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
