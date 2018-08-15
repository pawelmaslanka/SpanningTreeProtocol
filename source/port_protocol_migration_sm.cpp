// This project's headers
#include "stp/sm/port_protocol_migration.hpp"
// Dependencies
#include "stp/perf_params.hpp"
#include "stp/sm_conditions.hpp"

namespace Stp {
namespace PortProtocolMigration {

void PpmState::CheckingRstpAction(MachineH machine) {
    machine.PortInstance().SetMcheck(false);
    machine.PortInstance().SetSendRstp(SmConditions::RstpVersion(machine.BridgeInstance()));
    machine.PortInstance().SmTimersInstance().SetMdelayWhile(PerfParams::MigrateTime());
}

void PpmState::SelectingStpAction(MachineH machine) {
    machine.PortInstance().SetSendRstp(false);
    machine.PortInstance().SmTimersInstance().SetMdelayWhile(PerfParams::MigrateTime());
}

void PpmState::SensingAction(MachineH machine) {
    machine.PortInstance().SetRcvdRstp(false);
    machine.PortInstance().SetRcvdStp(false);
}

StateH BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(MachineH machine) {
    if (GoToCheckingRstp(machine)) {
        CheckingRstpAction(machine);
        ChangeState(machine, CheckingRstpState::Instance());
    }
}

bool BeginState::GoToCheckingRstp(MachineH machine) {
    return machine.BridgeInstance().Begin();
}

StateH CheckingRstpState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(CheckingRstpState);
}

void CheckingRstpState::Execute(MachineH machine) {
    if (GoToSensing(machine)) {
        SensingAction(machine);
        ChangeState(machine, SensingState::Instance());
    }
    else if (GoToCheckingRstp(machine)) {
        CheckingRstpAction(machine);
        // Leave it as current state
    }
}

bool CheckingRstpState::GoToCheckingRstp(MachineH machine) {
    if (machine.PortInstance().PortEnabled()) {
        return false;
    }
    else if (machine.PortInstance().SmTimersInstance().MdelayWhile() == PerfParams::MigrateTime()) {
        return false;
    }

    return true;
}

bool CheckingRstpState::GoToSensing(MachineH machine) {
    return SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().MdelayWhile());
}

StateH SensingState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(SensingState);
}

void SensingState::Execute(MachineH machine) {
    if (GoToCheckingRstp(machine)) {
        CheckingRstpAction(machine);
        ChangeState(machine, CheckingRstpState::Instance());
    }
    else if (GoToSelectingStp(machine)) {
        SelectingStpAction(machine);
        ChangeState(machine, SelectingStpState::Instance());
    }
}

bool SensingState::GoToCheckingRstp(MachineH machine) {
    if (not machine.PortInstance().PortEnabled()) {
        return true;
    }

    if (machine.PortInstance().Mcheck()) {
        return true;
    }

    if (not SmConditions::RstpVersion(machine.BridgeInstance())) {
        return false;
    }
    else if (machine.PortInstance().SendRstp()) {
        return false;
    }
    else if (not machine.PortInstance().RcvdRstp()) {
        return false;
    }

    return true;
}

bool SensingState::GoToSelectingStp(MachineH machine) {
    if (not machine.PortInstance().SendRstp()) {
        return false;
    }
    else if (not machine.PortInstance().RcvdStp()) {
        return false;
    }

    return true;
}

StateH SelectingStpState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(SelectingStpState);
}

void SelectingStpState::Execute(MachineH machine) {
    if (GoToSensing(machine)) {
        SensingAction(machine);
        ChangeState(machine, SensingState::Instance());
    }
}

bool SelectingStpState::GoToSensing(MachineH machine) {
    if (SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().MdelayWhile())) {
        return true;
    }

    if (not machine.PortInstance().PortEnabled()) {
        return true;
    }

    if (machine.PortInstance().Mcheck()) {
        return true;
    }

    return false;
}

} // namespace PortProtocolMigration
} // namespace SpanningTree
