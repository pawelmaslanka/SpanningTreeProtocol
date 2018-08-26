// This project's headers
#include "stp/sm/bridge_detection.hpp"
// Dependencies
#include "stp/sm_conditions.hpp"
#include "stp/sm_procedures.hpp"

namespace Stp {
namespace BridgeDetection {

void BdmState::EdgeAction(Machine& machine) {
    machine.PortInstance().SetOperEdge(true);
}

void BdmState::NotEdgeAction(Machine& machine) {
    machine.PortInstance().SetOperEdge(false);
}

State& BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(Machine& machine) {
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

bool BeginState::GoToEdge(Machine& machine) {
    if (not SmConditions::AdminEdge(machine.PortInstance())) {
        return false;
    }
    else {
        return true;
    }
}

State& EdgeState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(EdgeState);
}

void EdgeState::Execute(Machine& machine) {
    if (GoToNotEdge(machine)) {
        NotEdgeAction(machine);
        ChangeState(machine, NotEdgeState::Instance());
    }
}

bool EdgeState::GoToNotEdge(Machine& machine) {
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

State& NotEdgeState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(NotEdgeState);
}

void NotEdgeState::Execute(Machine& machine) {
    if (GoToEdge(machine)) {
        EdgeAction(machine);
        ChangeState(machine, EdgeState::Instance());
    }
}

bool NotEdgeState::GoToEdge(Machine& machine) {
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
} // namespace Stp
