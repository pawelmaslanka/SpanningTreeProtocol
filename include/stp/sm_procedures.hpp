#pragma once

// This project's headers
#include "port.hpp"

namespace Stp {
namespace SmProcedures {
//class SmProcedures {
//public:
    bool AdminEdge(PortH port) noexcept;
    bool DesignatedPort(PortH port) noexcept;
    bool RootPort(PortH port) noexcept;
    void TxConfig(PortH port);
    void TxTcn(PortH port);
    void TxRstp(PortH port);
    void UpdtBpduVersion(PortH port) noexcept;

    inline bool AdminEdge(PortH port) noexcept {
        return port.adminEdge;
    }

    inline bool DesignatedPort(PortH port) noexcept {
        return port.Role() == PortRole::Designated;
    }

    inline bool RootPort(PortH port) noexcept {
        return port.Role() == PortRole::Root;
    }
//};

} // namespace SmProcedures
} // namespace SpanningTree
