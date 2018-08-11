// This project's headers
#include "stp/sm/port_timers.hpp"

namespace SpanningTree {
namespace PortTimers {

void PtiTimers::OneSecondAction(MachineH machine) {
    machine.PortInstance().SetTick(false);
}

void PtiTimers::TickAction(MachineH machine) {
    --machine.PortInstance().SmTimersInstance();
    machine.PortInstance().DecTxCount();
    machine.PortInstance().DecAgeingTime();
}

StateH BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(MachineH machine) {
    if (GoToOneSecond(machine)) {
        OneSecondAction(machine);
        ChangeState(machine, OneSecondState::Instance());
    }
}

bool BeginState::GoToOneSecond(MachineH machine) {
    return machine.BridgeInstance().Begin();
}

StateH OneSecondState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(OneSecondState);
}

void OneSecondState::Execute(MachineH machine) {
    if (GoToTick(machine)) {
        TickAction(machine);
        ChangeState(machine, TickState::Instance());
    }
}

bool OneSecondState::GoToTick(MachineH machine) {
    return machine.PortInstance().Tick();
}

StateH TickState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TickState);
}

void TickState::Execute(MachineH machine) {
    if (GoToOneSecond(machine)) {
        OneSecondAction(machine);
        ChangeState(machine, OneSecondState::Instance());
    }
}

bool TickState::GoToOneSecond(MachineH machine) {
    std::ignore = machine;
    return true; // UCT
}

} // namespace PortTimers
} // namespace SpanningTree
