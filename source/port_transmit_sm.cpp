/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// This project's headers
#include "stp/sm/port_transmit.hpp"
// Dependencies
#include "stp/perf_params.hpp"
#include "stp/sm_conditions.hpp"
#include "stp/sm_procedures.hpp"
#include "stp/time.hpp"

namespace Stp {
namespace PortTransmit {

void PtxState::TransmitInitAction(Machine& machine) {
    machine.PortInstance().SetNewInfo(true);
    machine.PortInstance().SetTxCount(0);
}

void PtxState::TransmitPeriodicAction(Machine& machine) {
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

void PtxState::TransmitConfigAction(Machine& machine) {
    machine.PortInstance().SetNewInfo(false);
    SmProcedures::TxConfig(machine.BridgeInstance(), machine.PortInstance());
    machine.PortInstance().IncTxCount();
    machine.PortInstance().SetTcAck(false);
}

void PtxState::TransmitTcnAction(Machine& machine) {
    machine.PortInstance().SetNewInfo(false);
    SmProcedures::TxTcn(machine.BridgeInstance(), machine.PortInstance());
    machine.PortInstance().IncTxCount();
}

void PtxState::TransmitRstpAction(Machine& machine) {
    machine.PortInstance().SetNewInfo(false);
    SmProcedures::TxRstp(machine.BridgeInstance(), machine.PortInstance());
    machine.PortInstance().IncTxCount();
    machine.PortInstance().SetTcAck(false);
}

inline bool PtxState::GoToIdle(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

void PtxState::IdleAction(Machine& machine) {
    machine.PortInstance().SmTimersInstance().SetHelloWhen(
                PerfParams::HelloTime(machine.PortInstance()));
}

void PtxState::IdleUctExecute(Machine& machine) {
    if (GoToIdle(machine)) {
        IdleAction(machine);
        ChangeState(machine, IdleState::Instance());
    }
}

bool PtxState::TransitionQualified(Machine& machine) {
    if (not machine.PortInstance().Selected()) {
        return false;
    }

    if (machine.PortInstance().UpdtInfo()) {
        return false;
    }

    return true;
}

State& BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());

    if (GoToTransmitInit(machine)) {
        TransmitInitAction(machine);
        ChangeState(machine, TransmitInitState::Instance());
    }
}

bool BeginState::GoToTransmitInit(Machine& machine) {
    return machine.BridgeInstance().Begin();
}

State& TransmitInitState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TransmitInitState);
}

void TransmitInitState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());
    IdleUctExecute(machine);
}

State& TransmitPeriodicState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TransmitPeriodicState);
}

void TransmitPeriodicState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());
    IdleUctExecute(machine);
}

State& TransmitConfigState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TransmitConfigState);
}

void TransmitConfigState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());
    IdleUctExecute(machine);
}

State& TransmitTcnState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TransmitTcnState);
}

void TransmitTcnState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());
    IdleUctExecute(machine);
}

State& TransmitRstpState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TransmitRstpState);
}

void TransmitRstpState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());
    IdleUctExecute(machine);
}

State& IdleState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(IdleState);
}

void IdleState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());

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

bool IdleState::GoToTransmitPeriodic(Machine& machine) {
    if (not TransitionQualified(machine)) {
        return false;
    }
    else if (not SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().HelloWhen())) {
        return false;
    }

    return true;
}

bool IdleState::GoToTransmitConfig(Machine& machine) {
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

bool IdleState::GoToTransmitTcn(Machine& machine) {
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

bool IdleState::GoToTransmitRstp(Machine& machine) {
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
} // namespace Stp
