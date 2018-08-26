// This project's headers
#include "stp/sm/port_timers.hpp"

namespace Stp {
namespace PortTimers {

void PtiTimers::OneSecondAction(Machine& machine) {
    machine.PortInstance().SetTick(false);
}

void PtiTimers::TickAction(Machine& machine) {
    --machine.PortInstance().SmTimersInstance();
    machine.PortInstance().DecTxCount();
    machine.PortInstance().DecAgeingTime();
}

State& BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(Machine& machine) {
    if (GoToOneSecond(machine)) {
        OneSecondAction(machine);
        ChangeState(machine, OneSecondState::Instance());
    }
}

bool BeginState::GoToOneSecond(Machine& machine) {
    return machine.BridgeInstance().Begin();
}

State& OneSecondState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(OneSecondState);
}

void OneSecondState::Execute(Machine& machine) {
    if (GoToTick(machine)) {
        TickAction(machine);
        ChangeState(machine, TickState::Instance());
    }
}

bool OneSecondState::GoToTick(Machine& machine) {
    return machine.PortInstance().Tick();
}

State& TickState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(TickState);
}

void TickState::Execute(Machine& machine) {
    if (GoToOneSecond(machine)) {
        OneSecondAction(machine);
        ChangeState(machine, OneSecondState::Instance());
    }
}

inline bool TickState::GoToOneSecond(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

} // namespace PortTimers
} // namespace Stp
