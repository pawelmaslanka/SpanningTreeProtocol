/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// This project's headers
#include "stp/sm/port_receive.hpp"

#include "stp/perf_params.hpp"
#include "stp/sm_procedures.hpp"

namespace Stp {
namespace PortReceive {

void PrxState::DiscardAction(Machine& machine) {
    machine.PortInstance().SetRcvdBpdu(false);
    machine.PortInstance().SetRcvdRstp(false);
    machine.PortInstance().SetRcvdStp(false);
    machine.PortInstance().SetRcvdMsg(false);
    machine.PortInstance().SmTimersInstance().SetEdgeDelayWhile(PerfParams::MigrateTime());
}

void PrxState::ReceiveAction(Machine& machine) {
    SmProcedures::UpdtBpduVersion(machine.PortInstance());
    machine.PortInstance().SetOperEdge(false);
    machine.PortInstance().SetRcvdBpdu(false);
    machine.PortInstance().SetRcvdMsg(true);
    machine.PortInstance().SmTimersInstance().SetEdgeDelayWhile(PerfParams::MigrateTime());
}

State& BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());

    if (GoToDiscard(machine)) {
        DiscardAction(machine);
        ChangeState(machine, DiscardState::Instance());
    }
}

bool BeginState::GoToDiscard(Machine& machine) {
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

State& DiscardState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DiscardState);
}

void DiscardState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());

    if (GoToReceive(machine)) {
        ReceiveAction(machine);
        ChangeState(machine, ReceiveState::Instance());
    }
}

bool DiscardState::GoToReceive(Machine& machine) {
    if (not machine.PortInstance().RcvdBpdu()) {
        return false;
    }

    if (not machine.PortInstance().PortEnabled()) {
        return false;
    }

    return true;
}

State& ReceiveState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(ReceiveState);
}

void ReceiveState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());

    if (GoToReceive(machine)) {
        ReceiveAction(machine);
        // Leave it as current state
    }
}

bool ReceiveState::GoToReceive(Machine& machine) {
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
