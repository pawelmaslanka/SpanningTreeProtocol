// This project's headers
#include "stp/sm/port_role_selection.hpp"
// Dependencies
#include "stp/sm_procedures.hpp"

namespace Stp {
namespace PortRoleSelection {

void PrsState::InitBridgeAction(Machine& machine) {
    SmProcedures::UpdtRoleDisabledTree(machine.BridgeInstance());
}

void PrsState::RoleSelectionAction(Machine& machine) {
    SmProcedures::ClearReselectTree(machine.BridgeInstance());
    SmProcedures::UpdtRolesTree(machine.BridgeInstance());
    SmProcedures::SetSelectedTree(machine.BridgeInstance());
}

State& BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(Machine& machine) {
    if (GoToInitBridge(machine)) {
        InitBridgeAction(machine);
        ChangeState(machine, InitBridgeState::Instance());
    }
}

bool BeginState::GoToInitBridge(Machine& machine) {
    return machine.BridgeInstance().Begin();
}

State& InitBridgeState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(InitBridgeState);
}

void InitBridgeState::Execute(Machine& machine) {
    if (GoToRoleSelection(machine)) {
        RoleSelectionAction(machine);
        ChangeState(machine, RoleSelectionState::Instance());
    }
}

bool InitBridgeState::GoToRoleSelection(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& RoleSelectionState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(RoleSelectionState);
}

void RoleSelectionState::Execute(Machine& machine) {
    if (GoToRoleSelection(machine)) {
        RoleSelectionAction(machine);
        // Leave it as the current state
    }
}

bool RoleSelectionState::GoToRoleSelection(Machine& machine) {
    for (const auto& portMapIt : machine.BridgeInstance().GetAllPorts()) {
        if (portMapIt.second->Reselect()) {
            return true;;
        }
    }

    return false;
}

} // namespace PortTransmit
} // namespace Stp
