// This project's headers
#include "stp/sm/port_transmit.hpp"
// Dependencies
#include "stp/perf_params.hpp"
#include "stp/sm_conditions.hpp"
#include "stp/sm_procedures.hpp"
#include "stp/time.hpp"

namespace Stp {
namespace PortTransmit {

void PtxState::TransmitInitAction(MachineH machine) {
    machine.PortInstance().SetNewInfo(true);
    machine.PortInstance().SetTxCount(0);
}

void PtxState::TransmitPeriodicAction(MachineH machine) {
    bool newInfo = machine.PortInstance().NewInfo();

    if (not newInfo) {
        newInfo = SmProcedures::DesignatedPort(machine.PortInstance());
    }

    if (not newInfo) {
        newInfo = SmProcedures::RootPort(machine.PortInstance());

        if (newInfo) {
            newInfo = not SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().TcWhile());
        }
    }

    machine.PortInstance().SetNewInfo(newInfo);
}

void PtxState::TransmitConfigAction(MachineH machine) {
    machine.PortInstance().SetNewInfo(false);
    SmProcedures::TxConfig(machine.PortInstance());
    machine.PortInstance().IncTxCount();
    machine.PortInstance().SetTcAck(false);
}

void PtxState::TransmitTcnAction(MachineH machine) {
    machine.PortInstance().SetNewInfo(false);
    SmProcedures::TxTcn(machine.PortInstance());
    machine.PortInstance().IncTxCount();
}

void PtxState::TransmitRstpAction(MachineH machine) {
    machine.PortInstance().SetNewInfo(false);
    SmProcedures::TxRstp(machine.PortInstance());
    machine.PortInstance().IncTxCount();
    machine.PortInstance().SetTcAck(false);
}

inline bool PtxState::GoToIdle(MachineH machine) {
    std::ignore = machine;
    return true; // UCT
}

void PtxState::IdleAction(MachineH machine) {
    machine.PortInstance().SmTimersInstance().SetHelloWhen(
                PerfParams::HelloTime(machine.PortInstance()));
}

void PtxState::IdleUctExecute(MachineH machine) {
    if (GoToIdle(machine)) {
        IdleAction(machine);
        ChangeState(machine, IdleState::Instance());
    }
}

bool PtxState::TransitionQualified(MachineH machine) {
    if (not machine.PortInstance().Selected()) {
        return false;
    }

    if (machine.PortInstance().UpdtInfo()) {
        return false;
    }

    return true;
}

StateH BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(MachineH machine) {
    if (GoToTransmitInit(machine)) {
        TransmitInitAction(machine);
        ChangeState(machine, TransmitInitState::Instance());
    }
}

bool BeginState::GoToTransmitInit(MachineH machine) {
    return machine.BridgeInstance().Begin();
}

StateH TransmitInitState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TransmitInitState);
}

void TransmitInitState::Execute(MachineH machine) {
    IdleUctExecute(machine);
}

StateH TransmitPeriodicState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TransmitPeriodicState);
}

void TransmitPeriodicState::Execute(MachineH machine) {
    IdleUctExecute(machine);
}

StateH TransmitConfigState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TransmitConfigState);
}

void TransmitConfigState::Execute(MachineH machine) {
    IdleUctExecute(machine);
}

StateH TransmitTcnState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TransmitTcnState);
}

void TransmitTcnState::Execute(MachineH machine) {
    IdleUctExecute(machine);
}

StateH TransmitRstpState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TransmitRstpState);
}

void TransmitRstpState::Execute(MachineH machine) {
    IdleUctExecute(machine);
}

StateH IdleState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(IdleState);
}

void IdleState::Execute(MachineH machine) {
    if (GoToTransmitPeriodic(machine)) {
        TransmitPeriodicAction(machine);
        ChangeState(machine, TransmitPeriodicState::Instance());
    }
    else if (GoToTransmitConfig(machine)) {
        TransmitConfigAction(machine);
        ChangeState(machine, TransmitConfigState::Instance());
    }
    else if (GoToTransmitTcn(machine)) {
        TransmitTcnAction(machine);
        ChangeState(machine, TransmitTcnState::Instance());
    }
    else if (GoToTransmitRstp(machine)) {
        TransmitRstpAction(machine);
        ChangeState(machine, TransmitRstpState::Instance());
    }
}

bool IdleState::GoToTransmitPeriodic(MachineH machine) {
    if (not TransitionQualified(machine)) {
        return false;
    }
    else if (not SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().HelloWhen())) {
        return false;
    }

    return true;
}

bool IdleState::GoToTransmitConfig(MachineH machine) {
    if (not TransitionQualified(machine)) {
        return false;
    }
    else if (machine.PortInstance().SendRstp()) {
        return false;
    }
    else if (not machine.PortInstance().NewInfo()) {
        return false;
    }
    else if (not SmProcedures::DesignatedPort(machine.PortInstance())) {
        return false;
    }
    else if (not (machine.PortInstance().TxCount() < PerfParams::TxHoldCount())) {
        return false;
    }
    else if (SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().HelloWhen())) {
        return false;
    }

    return true;
}

bool IdleState::GoToTransmitTcn(MachineH machine) {
    if (not TransitionQualified(machine)) {
        return false;
    }
    else if (machine.PortInstance().SendRstp()) {
        return false;
    }
    else if (not machine.PortInstance().NewInfo()) {
        return false;
    }
    else if (not SmProcedures::RootPort(machine.PortInstance())) {
        return false;
    }
    else if (not (machine.PortInstance().TxCount() < PerfParams::TxHoldCount())) {
        return false;
    }
    else if (SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().HelloWhen())) {
        return false;
    }

    return true;
}

bool IdleState::GoToTransmitRstp(MachineH machine) {
    if (not TransitionQualified(machine)) {
        return false;
    }
    else if (not machine.PortInstance().SendRstp()) {
        return false;
    }
    else if (not machine.PortInstance().NewInfo()) {
        return false;
    }
    else if (not (machine.PortInstance().TxCount() < PerfParams::TxHoldCount())) {
        return false;
    }
    else if (SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().HelloWhen())) {
        return false;
    }

    return true;
}

} // namespace PortTransmit
} // namespace SpanningTree
