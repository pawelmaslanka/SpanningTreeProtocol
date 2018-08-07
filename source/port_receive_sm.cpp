// This project's headers
#include "port_receive_sm.hpp"

#include "perf_params.hpp"
#include "sm_procedures.hpp"

static std::string SmPrefixName { "PRX @ " };

namespace SpanningTree {
namespace PortReceive {

void PrxState::DiscardAction(MachineH machine) {
    machine.PortInstance().SetRcvdBpdu(false);
    machine.PortInstance().SetRcvdMsg(false);
    machine.PortInstance().SetRcvdRstp(false);
    machine.PortInstance().SetRcvdStp(false);
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
    return (machine.BridgeInstance().Begin()
            || ((machine.PortInstance().RcvdBpdu()
                 || (machine.PortInstance().SmTimersInstance().EdgeDelayWhile()
                     != PerfParams::MigrateTime()))
                && not machine.PortInstance().PortEnabled()))
            ? true
            : false;
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
    return (machine.PortInstance().RcvdBpdu() && machine.PortInstance().PortEnabled()
            && not machine.PortInstance().RcvdMsg())
            ? true
            : false;
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
    return (machine.PortInstance().RcvdBpdu() && machine.PortInstance().PortEnabled()
            && not machine.PortInstance().RcvdMsg())
            ? true
            : false;
}

}
}
