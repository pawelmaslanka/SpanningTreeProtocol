#pragma once

// This project's headers
#include "bridge.hpp"
#include "port.hpp"

namespace Stp {
namespace SmProcedures {

bool AdminEdge(PortH port) noexcept;
bool BetterOrSameInfo(PortH port, const Port::Info newInfoIs) noexcept;
bool DesignatedPort(PortH port) noexcept;
void RecordAgreement(BridgeH bridge, PortH port) noexcept;
void RecordDispute(PortH port) noexcept;
void RecordPriority(PortH port) noexcept;
void RecordProposal(PortH port) noexcept;
void RecordTimes(PortH port) noexcept;
bool RootPort(PortH port) noexcept;
void SetTcFlags(PortH port) noexcept;
void TxConfig(PortH port);
void TxRstp(PortH port);
void TxTcn(PortH port);
void UpdtBpduVersion(PortH port) noexcept;
void UpdtRcvdInfoWhile(PortH port) noexcept;

inline bool AdminEdge(PortH port) noexcept {
    return port.adminEdge;
}

inline bool DesignatedPort(PortH port) noexcept {
    return port.Role() == PortRole::Designated;
}

inline bool RootPort(PortH port) noexcept {
    return port.Role() == PortRole::Root;
}

} // namespace SmProcedures
} // namespace SpanningTree
