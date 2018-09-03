/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// This project's headers
#include "stp/sm/topology_change.hpp"
// Dependencies
#include "stp/sm_conditions.hpp"
#include "stp/sm_parameters.hpp"
#include "stp/sm_procedures.hpp"

namespace Stp {
namespace TopologyChange {

void TcmState::InactiveAction(Machine& machine) {
    /// @todo Do we need really here to flush entries data base?
    SmProcedures::FlushFdb(machine.BridgeInstance(), machine.PortInstance());
    machine.PortInstance().SetFdbFlush(true);
    machine.PortInstance().SmTimersInstance().SetTcWhile(0);
    machine.PortInstance().SetTcAck(false);
    // Exception: 17.19.1
    machine.PortInstance().SetAgeingTime(Bridge::AgeingTime);
    if (SmConditions::StpVersion(machine.BridgeInstance())) {
        machine.PortInstance().SetAgeingTime(SmParams::FwdDelay(machine.PortInstance()));
    }
}

void TcmState::LearningAction(Machine& machine) {
    machine.PortInstance().SetRcvdTc(false);
    machine.PortInstance().SetRcvdTcn(false);
    machine.PortInstance().SetRcvdTcAck(false);
    machine.PortInstance().SetTcProp(false);
}

void TcmState::DetectedAction(Machine& machine) {
    SmProcedures::NewTcWhile(machine.BridgeInstance(), machine.PortInstance());
    SmProcedures::SetTcPropTree(machine.BridgeInstance(), machine.PortInstance());
    machine.PortInstance().SetNewInfo(true);
}

void TcmState::NotifiedTcnAction(Machine& machine) {
    SmProcedures::NewTcWhile(machine.BridgeInstance(), machine.PortInstance());
}

void TcmState::NotifiedTcAction(Machine& machine) {
    machine.PortInstance().SetRcvdTc(false);
    machine.PortInstance().SetRcvdTcn(false);
    if (PortRole::Designated == machine.PortInstance().Role()) {
        machine.PortInstance().SetTcAck(true);
    }

    SmProcedures::SetTcPropTree(machine.BridgeInstance(), machine.PortInstance());
}

void TcmState::PropagatingAction(Machine& machine) {
    SmProcedures::NewTcWhile(machine.BridgeInstance(), machine.PortInstance());
    SmProcedures::FlushFdb(machine.BridgeInstance(), machine.PortInstance());
    machine.PortInstance().SetFdbFlush(true);
    machine.PortInstance().SetTcProp(false);
    // Exception: 17.19.1
    machine.PortInstance().SetAgeingTime(Bridge::AgeingTime);
    if (SmConditions::StpVersion(machine.BridgeInstance())) {
        machine.PortInstance().SetAgeingTime(SmParams::FwdDelay(machine.PortInstance()));
    }
}

void TcmState::AcknowledgeAction(Machine& machine) {
    machine.PortInstance().SmTimersInstance().SetTcWhile(0);
    machine.PortInstance().SetRcvdTcAck(false);
}

void TcmState::ActiveAction(Machine& machine) {
    std::ignore = machine;
    return;
}

void TcmState::ActiveUctExecute(Machine& machine) {
    std::ignore = machine;
}

State& BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());

    if (GoToInactive(machine)) {
        InactiveAction(machine);
        ChangeState(machine, InactiveState::Instance());
    }
}

bool BeginState::GoToInactive(Machine& machine) {
    return machine.BridgeInstance().Begin();
}

State& InactiveState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(InactiveState);
}

void InactiveState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());

    if (GoToLearning(machine)) {
        LearningAction(machine);
        ChangeState(machine, LearningState::Instance());
    }
}

bool InactiveState::GoToLearning(Machine& machine) {
    if (not machine.PortInstance().Learn()) {
        return false;
    }
    else if (machine.PortInstance().FdbFlush()) {
        return false;
    }

    return true;
}

State& LearningState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(LearningState);
}

void LearningState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());

    if (GoToDetected(machine)) {
        DetectedAction(machine);
        ChangeState(machine, DetectedState::Instance());
    }
    else if (GoToInactive(machine)) {
        InactiveAction(machine);
        ChangeState(machine, InactiveState::Instance());
    }
    else if (GoToLearning(machine)) {
        LearningAction(machine);
        // Leave it as a current state
    }
}

bool LearningState::GoToDetected(Machine& machine) {
    if (not machine.PortInstance().Forward()) {
        return false;
    }
    else if (machine.PortInstance().OperEdge()) {
        return false;
    }

    if (PortRole::Root == machine.PortInstance().Role()) {
        return true;
    }
    else if (PortRole::Designated == machine.PortInstance().Role()) {
        return true;
    }

    return false;
}

bool LearningState::GoToInactive(Machine& machine) {
    if (PortRole::Root == machine.PortInstance().Role()) {
        return false;
    }
    else if (PortRole::Designated == machine.PortInstance().Role()) {
        return false;
    }
    else if (machine.PortInstance().Learn()) {
        return false;
    }
    else if (machine.PortInstance().Learning()) {
        return false;
    }
    else if (machine.PortInstance().RcvdTc()) {
        return false;
    }
    else if (machine.PortInstance().RcvdTcn()) {
        return false;
    }
    else if (machine.PortInstance().RcvdTcAck()) {
        return false;
    }
    else if (machine.PortInstance().TcProp()) {
        return false;
    }

    return true;
}

bool LearningState::GoToLearning(Machine& machine) {
    if (machine.PortInstance().RcvdTc()) {
        return true;
    }
    else if (machine.PortInstance().RcvdTcn()) {
        return true;
    }
    else if (machine.PortInstance().RcvdTcAck()) {
        return true;
    }
    else if (machine.PortInstance().TcProp()) {
        return true;
    }

    return false;
}

State& DetectedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DetectedState);
}

void DetectedState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());
    ActiveUctExecute(machine);
}

State& NotifiedTcnState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(NotifiedTcnState);
}

void NotifiedTcnState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());

    if (GoToNotifiedTc(machine)) {
        NotifiedTcAction(machine);
        ChangeState(machine, NotifiedTcState::Instance());
    }
}

bool NotifiedTcnState::GoToNotifiedTc(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& NotifiedTcState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(NotifiedTcState);
}

void NotifiedTcState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());
    ActiveUctExecute(machine);
}

State& PropagatingState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(PropagatingState);
}

void PropagatingState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());
    ActiveUctExecute(machine);
}

State& AcknowledgedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(AcknowledgedState);
}

void AcknowledgedState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());
    ActiveUctExecute(machine);
}

State& ActiveState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(ActiveState);
}

void ActiveState::Execute(Machine& machine) {
    machine.BridgeInstance().SystemLogEntryState(machine.Name(), Name());

    if (GoToLearning(machine)) {
        LearningAction(machine);
        ChangeState(machine, LearningState::Instance());
    }
    else if (GoToNotifiedTcn(machine)) {
        NotifiedTcnAction(machine);
        ChangeState(machine, NotifiedTcnState::Instance());
    }
    else if (GoToNotifiedTc(machine)) {
        NotifiedTcAction(machine);
        ChangeState(machine, NotifiedTcState::Instance());
    }
    else if (GoToPropagating(machine)) {
        PropagatingAction(machine);
        ChangeState(machine, PropagatingState::Instance());
    }
    else if (GoToAcknowledged(machine)) {
        AcknowledgeAction(machine);
        ChangeState(machine, AcknowledgedState::Instance());
    }
}

bool ActiveState::GoToLearning(Machine& machine) {
    if (machine.PortInstance().OperEdge()) {
        return true;
    }

    if (PortRole::Root == machine.PortInstance().Role()) {
        return false;
    }
    else if (PortRole::Designated == machine.PortInstance().Role()) {
        return false;
    }

    return true;
}

bool ActiveState::GoToNotifiedTcn(Machine& machine) {
    return machine.PortInstance().RcvdTcn();
}

bool ActiveState::GoToNotifiedTc(Machine& machine) {
    return machine.PortInstance().RcvdTc();
}

bool ActiveState::GoToPropagating(Machine& machine) {
    if (not machine.PortInstance().TcProp()) {
        return false;
    }
    else if (machine.PortInstance().OperEdge()) {
        return false;
    }

    return true;
}

bool ActiveState::GoToAcknowledged(Machine& machine) {
    return machine.PortInstance().RcvdTcAck();
}

} // namespace TopologyChange
} // namespace Stp
