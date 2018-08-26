#pragma once

// This project's headers
#include "bridge.hpp"
#include "port.hpp"

namespace Stp {
namespace SmProcedures {

bool AdminEdge(Port& port) noexcept;
bool AllSynced(Bridge& bridge) noexcept;
bool BetterOrSameInfo(Port& port, const Port::Info newInfoIs) noexcept;
void ClearReselectTree(Bridge& bridge) noexcept;
bool DesignatedPort(Port& port) noexcept;
void DisableForwarding(Bridge& bridge, const Port& port) noexcept;
void DisableLearning(Bridge& bridge, const Port& port) noexcept;
void EnableForwarding(Bridge& bridge, const Port& port) noexcept;
void EnableLearning(Bridge& bridge, const Port& port) noexcept;
void FlushFdb(Bridge& bridge, const Port& port) noexcept;
u16 MaxAge(const Port& port) noexcept;
void NewTcWhile(const Bridge& bridge, Port& port) noexcept;
void RecordAgreement(Bridge& bridge, Port& port) noexcept;
void RecordDispute(Port& port) noexcept;
void RecordPriority(Port& port) noexcept;
void RecordProposal(Port& port) noexcept;
void RecordTimes(Port& port) noexcept;
bool RootPort(Port& port) noexcept;
void SetReRootTree(Bridge& bridge) noexcept;
void SetSelectedTree(Bridge& bridge) noexcept;
void SetSyncTree(Bridge& bridge) noexcept;
void SetTcFlags(Port& port) noexcept;
void SetTcPropTree(Bridge& bridge, const Port& port) noexcept;
void TxConfig(Bridge& bridge, Port& port);
void TxRstp(Bridge& bridge, Port& port);
void TxTcn(Bridge& bridge, Port& port);
void UpdtBpduVersion(Port& port) noexcept;
void UpdtRcvdInfoWhile(Port& port) noexcept;
void UpdtRoleDisabledTree(Bridge& bridge) noexcept;
void UpdtRolesTree(Bridge& bridge) noexcept;

inline bool AdminEdge(Port& port) noexcept {
    return port.adminEdge;
}

inline bool DesignatedPort(Port& port) noexcept {
    return port.Role() == PortRole::Designated;
}

inline void DisableForwarding(Bridge& bridge, const Port& port) noexcept {
    bridge.SetForwarding(port.PortId().PortNum(), false);
}

inline void DisableLearning(Bridge& bridge, const Port& port) noexcept {
    bridge.SetLearning(port.PortId().PortNum(), false);
}

inline void EnableForwarding(Bridge& bridge, const Port& port) noexcept {
    bridge.SetForwarding(port.PortId().PortNum(), true);
}

inline void EnableLearning(Bridge& bridge, const Port& port) noexcept {
    bridge.SetLearning(port.PortId().PortNum(), true);
}

inline bool RootPort(Port& port) noexcept {
    return port.Role() == PortRole::Root;
}

} // namespace SmProcedures
} // namespace Stp
