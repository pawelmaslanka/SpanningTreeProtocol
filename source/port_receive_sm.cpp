// This project's headers
#include "stp/sm/port_receive.hpp"

#include "stp/perf_params.hpp"
#include "stp/sm_procedures.hpp"

namespace Stp {
namespace PortReceive {

void PrxState::DiscardAction(MachineH machine) {
    machine.PortInstance().SetRcvdBpdu(false);
    machine.PortInstance().SetRcvdRstp(false);
    machine.PortInstance().SetRcvdStp(false);
    machine.PortInstance().SetRcvdMsg(false);
    machine.PortInstance().SmTimersInstance().SetEdgeDelayWhile(PerfParams::MigrateTime());
}

void PrxState::ReceiveAction(MachineH machine) {
    SmProcedures::UpdtBpduVersion(machine.PortInstance());
    machine.PortInstance().SetOperEdge(false);
    machine.PortInstance().SetRcvdBpdu(false);
    machine.PortInstance().SetRcvdMsg(true);
    machine.PortInstance().SmTimersInstance().SetEdgeDelayWhile(PerfParams::MigrateTime());
}

StateH BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(MachineH machine) {
    if (GoToDiscard(machine)) {
        DiscardAction(machine);
        ChangeState(machine, DiscardState::Instance());
    }
}

bool BeginState::GoToDiscard(MachineH machine) {
    if (machine.BridgeInstance().Begin()) {
        return true;
    }

    if (not machine.PortInstance().PortEnabled()) {
        if (machine.PortInstance().RcvdBpdu()) {
            return true;
        }

        if (machine.PortInstance().SmTimersInstance().EdgeDelayWhile()
                != PerfParams::MigrateTime()) {
            return true;
        }
    }

    return false;
}

StateH DiscardState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DiscardState);
}

void DiscardState::Execute(MachineH machine) {
    if (GoToReceive(machine)) {
        ReceiveAction(machine);
        ChangeState(machine, ReceiveState::Instance());
    }
}

bool DiscardState::GoToReceive(MachineH machine) {
    if (not machine.PortInstance().RcvdBpdu()) {
        return false;
    }

    if (not machine.PortInstance().PortEnabled()) {
        return false;
    }

    return true;
}

StateH ReceiveState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(ReceiveState);
}

void ReceiveState::Execute(MachineH machine) {
    if (GoToReceive(machine)) {
        ReceiveAction(machine);
        // Leave it as current state
    }
}

bool ReceiveState::GoToReceive(MachineH machine) {
    if (not machine.PortInstance().RcvdBpdu()) {
        return false;
    }

    if (not machine.PortInstance().PortEnabled()) {
        return false;
    }

    if (machine.PortInstance().RcvdMsg()) {
        return false;
    }

    return true;
}

}
}
