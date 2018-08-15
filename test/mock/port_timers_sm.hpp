#pragma once

// Tested project's headers
#include <stp/sm/port_timers.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {
namespace Pti {

using namespace Stp;

class BeginState : public PortTimers::BeginState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToOneSecond, bool(MachineH));
    MOCK_METHOD1(OneSecondAction, void(MachineH));
    MOCK_METHOD1(TickAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortTimers::BeginState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortTimers::BeginState::ChangeState(machine, newState);
    }
};

class OneSecondState : public PortTimers::OneSecondState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToTick, bool(MachineH));
    MOCK_METHOD1(OneSecondAction, void(MachineH));
    MOCK_METHOD1(TickAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortTimers::OneSecondState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortTimers::OneSecondState::ChangeState(machine, newState);
    }
};

class TickState : public PortTimers::TickState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToOneSecond, bool(MachineH));
    MOCK_METHOD1(OneSecondAction, void(MachineH));
    MOCK_METHOD1(TickAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortTimers::TickState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortTimers::TickState::ChangeState(machine, newState);
    }
};

} // namespace Prx
} // namespace Mock
