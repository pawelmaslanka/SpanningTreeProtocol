/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

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
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToOneSecond, bool(Machine&));
    MOCK_METHOD1(OneSecondAction, void(Machine&));
    MOCK_METHOD1(TickAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortTimers::BeginState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortTimers::BeginState::ChangeState(machine, newState);
    }
};

class OneSecondState : public PortTimers::OneSecondState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToTick, bool(Machine&));
    MOCK_METHOD1(OneSecondAction, void(Machine&));
    MOCK_METHOD1(TickAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortTimers::OneSecondState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortTimers::OneSecondState::ChangeState(machine, newState);
    }
};

class TickState : public PortTimers::TickState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToOneSecond, bool(Machine&));
    MOCK_METHOD1(OneSecondAction, void(Machine&));
    MOCK_METHOD1(TickAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortTimers::TickState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortTimers::TickState::ChangeState(machine, newState);
    }
};

} // namespace Prx
} // namespace Mock
