#pragma once

#include "bridge.hpp"
#include "lib.hpp"

namespace Stp {
namespace SmConditions {

bool AdminEdge(PortH port) noexcept;
bool AutoEdge(PortH port) noexcept;
enum Port::RcvdInfo RcvInfo(PortH port) noexcept;
bool RstpVersion(BridgeH bridge) noexcept;

inline bool AdminEdge(PortH port) noexcept {
    return port.adminEdge;
}

inline bool AutoEdge(PortH port) noexcept {
    return port.autoEdge;
}

inline bool RstpVersion(BridgeH bridge) noexcept {
    return bridge.ForceProtocolVersion >= 2;
}

} // namespace SmConditions
} // namespace Stp
