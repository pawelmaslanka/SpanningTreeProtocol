// This project's headers
#include "stp/sm/port_state_transition.hpp"
// Dependencies
#include "stp/sm_procedures.hpp"

namespace Stp {
namespace PortStateTransition {

void PstState::DiscardingAction(Machine& machine) {
    SmProcedures::DisableLearning(machine.BridgeInstance(), machine.PortInstance());
    machine.PortInstance().SetLearning(false);
    SmProcedures::DisableForwarding(machine.BridgeInstance(), machine.PortInstance());
    machine.PortInstance().SetForwarding(false);
}

void PstState::LearningAction(Machine& machine) {
    SmProcedures::EnableLearning(machine.BridgeInstance(), machine.PortInstance());
    machine.PortInstance().SetLearning(true);
    // Exception: 17.19.1
    machine.PortInstance().SetAgeingTime(Bridge::AgeingTime);
}

void PstState::ForwardingAction(Machine &machine) {
    SmProcedures::EnableForwarding(machine.BridgeInstance(), machine.PortInstance());
    machine.PortInstance().SetForwarding(true);
}

State& BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(Machine& machine) {
    if (GoToDiscarding(machine)) {
        DiscardingAction(machine);
        ChangeState(machine, DiscardingState::Instance());
    }
}

bool BeginState::GoToDiscarding(Machine& machine) {
    return machine.BridgeInstance().Begin();
}

State& DiscardingState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DiscardingState);
}

void DiscardingState::Execute(Machine& machine) {
    if (GoToLearning(machine)) {
        LearningAction(machine);
        ChangeState(machine, LearningState::Instance());
    }
}

bool DiscardingState::GoToLearning(Machine& machine) {
    return machine.PortInstance().Learn();
}

State& LearningState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(LearningState);
}

void LearningState::Execute(Machine& machine) {
    if (GoToForwarding(machine)) {
        ForwardingAction(machine);
        ChangeState(machine, ForwardingState::Instance());
    }
    else if (GoToDiscarding(machine)) {
        DiscardingAction(machine);
        ChangeState(machine, DiscardingState::Instance());
    }
}

bool LearningState::GoToForwarding(Machine& machine) {
    return machine.PortInstance().Forward();
}

bool LearningState::GoToDiscarding(Machine& machine) {
    return not machine.PortInstance().Learn();
}

State& ForwardingState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(ForwardingState);
}

void ForwardingState::Execute(Machine& machine) {
    if (GoToDiscarding(machine)) {
        DiscardingAction(machine);
        ChangeState(machine, DiscardingState::Instance());
    }
}

bool ForwardingState::GoToDiscarding(Machine& machine) {
    return not machine.PortInstance().Forward();
}

} // namespace BridgeDetection
} // namespace Stp
