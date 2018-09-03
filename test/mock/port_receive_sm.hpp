/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// Tested project's headers
#include <stp/sm/port_receive.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {
namespace Prx {

using namespace Stp;

class BeginState : public PortReceive::BeginState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToDiscard, bool(Machine&));
    MOCK_METHOD1(DiscardAction, void(Machine&));
    MOCK_METHOD1(ReceiveAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortReceive::BeginState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortReceive::BeginState::ChangeState(machine, newState);
    }
};

class DiscardState : public PortReceive::DiscardState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToReceive, bool(Machine&));
    MOCK_METHOD1(DiscardAction, void(Machine&));
    MOCK_METHOD1(ReceiveAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortReceive::DiscardState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortReceive::DiscardState::ChangeState(machine, newState);
    }
};

class ReceiveState : public PortReceive::ReceiveState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToReceive, bool(Machine&));
    MOCK_METHOD1(DiscardAction, void(Machine&));
    MOCK_METHOD1(ReceiveAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortReceive::ReceiveState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortReceive::ReceiveState::ChangeState(machine, newState);
    }
};

} // namespace Prx
} // namespace Mock

