// This project's headers
#include "port_protocol_migration_sm.hpp"

#include "perf_params.hpp"
#include "sm_conditions.hpp"

namespace SpanningTree {
namespace PortProtocolMigration {

void State::CheckingRstpAction(MachineH machine) {
    machine.PortInstance().SetMcheck(false);
    machine.PortInstance().SetSendRstp(SmConditions::RstpVersion(machine.BridgeInstance()));
    machine.PortInstance().SmTimersInstance().SetMdelayWhile(Time::RecommendedValue::MigrateTime);
}

void State::SelectingStpAction(MachineH machine) {
    machine.PortInstance().SetSendRstp(false);
    machine.PortInstance().SmTimersInstance().SetMdelayWhile(Time::RecommendedValue::MigrateTime);
}

void State::SensingAction(MachineH machine) {
    machine.PortInstance().SetRcvdRstp(false);
    machine.PortInstance().SetRcvdStp(false);
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
    return (machine.PortInstance().SmTimersInstance().MdelayWhile() != PerfParams::MigrateTime())
            && not machine.PortInstance().PortEnabled();
}

bool CheckingRstpState::GoToSensing(MachineH machine) {
    return not machine.PortInstance().PortEnabled() || machine.PortInstance().Mcheck()
            || (SmConditions::RstpVersion(machine.BridgeInstance())
                && not machine.PortInstance().SendRstp() && machine.PortInstance().RcvdRstp());
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
    return not machine.PortInstance().PortEnabled() || machine.PortInstance().Mcheck()
            || (SmConditions::RstpVersion(machine.BridgeInstance())
                && not machine.PortInstance().SendRstp() && machine.PortInstance().RcvdRstp());
}

bool SensingState::GoToSelectingStp(MachineH machine) {
    return machine.PortInstance().SendRstp() && machine.PortInstance().RcvdStp();}

void SelectingStpState::Execute(MachineH machine) {
    if (GoToSensing(machine)) {
        SensingAction(machine);
        ChangeState(machine, SensingState::Instance());
    }
}

bool SelectingStpState::GoToSensing(MachineH machine) {
    return SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().MdelayWhile())
            || not machine.PortInstance().PortEnabled() || machine.PortInstance().Mcheck();
}

} // namespace PortProtocolMigration
} // namespace SpanningTree
