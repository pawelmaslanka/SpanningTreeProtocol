#pragma once

#include "bridge.hpp"
#include "lib.hpp"

namespace Stp {

class SmConditions {
public:
    static bool AdminEdge(Port& port) noexcept;
    static bool AutoEdge(Port& port) noexcept;
    static bool RstpVersion(Bridge& bridge) noexcept;
};

inline bool SmConditions::AdminEdge(Port& port) noexcept {
    return port.adminEdge;
}

inline bool SmConditions::AutoEdge(Port& port) noexcept {
    return port.autoEdge;
}

inline bool SmConditions::RstpVersion(Bridge &bridge) noexcept {
    return bridge.ForceProtocolVersion >= 2;
}

} // namespace SpanningTree
