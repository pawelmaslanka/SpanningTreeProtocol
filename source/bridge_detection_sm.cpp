// This project's headers
#include "stp/sm/bridge_detection.hpp"
// Dependencies
#include "stp/sm_conditions.hpp"
#include "stp/sm_procedures.hpp"

namespace Stp {
namespace BridgeDetection {

void BdmState::EdgeAction(MachineH machine) {
    machine.PortInstance().SetOperEdge(true);
}

void BdmState::NotEdgeAction(MachineH machine) {
    machine.PortInstance().SetOperEdge(false);
}

StateH BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(MachineH machine) {
    if (not machine.BridgeInstance().Begin()) {
        return;
    }

    if (GoToEdge(machine)) {
        EdgeAction(machine);
        ChangeState(machine, EdgeState::Instance());
    }
    else {
        NotEdgeAction(machine);
        ChangeState(machine, NotEdgeState::Instance());
    }
}

bool BeginState::GoToEdge(MachineH machine) {
    if (not SmConditions::AdminEdge(machine.PortInstance())) {
        return false;
    }
    else {
        return true;
    }
}

StateH EdgeState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(EdgeState);
}

void EdgeState::Execute(MachineH machine) {
    if (GoToNotEdge(machine)) {
        NotEdgeAction(machine);
        ChangeState(machine, NotEdgeState::Instance());
    }
}

bool EdgeState::GoToNotEdge(MachineH machine) {
    if (not machine.PortInstance().OperEdge()) {
        return true;
    }

    if (machine.PortInstance().PortEnabled()) {
        return false;
    }
    else if (SmProcedures::AdminEdge(machine.PortInstance())) {
        return false;
    }

    return true;
}

StateH NotEdgeState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(NotEdgeState);
}

void NotEdgeState::Execute(MachineH machine) {
    if (GoToEdge(machine)) {
        EdgeAction(machine);
        ChangeState(machine, EdgeState::Instance());
    }
}

bool NotEdgeState::GoToEdge(MachineH machine) {
    if (not machine.PortInstance().PortEnabled()) {
        if (SmConditions::AdminEdge(machine.PortInstance())) {
            return true;
        }
    }

    if (not SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().EdgeDelayWhile())) {
        return false;
    }
    else if (not SmConditions::AutoEdge(machine.PortInstance())) {
        return false;
    }
    else if (not machine.PortInstance().SendRstp()) {
        return false;
    }
    else if (not machine.PortInstance().Proposing()) {
        return false;
    }

    return true;
}

} // namespace BridgeDetection
} // namespace SpanningTree
