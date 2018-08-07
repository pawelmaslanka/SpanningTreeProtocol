#pragma once

// Tested project's headers
#include <port_receive_sm.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {
namespace Prx {

using namespace SpanningTree;

class BeginState : public PortReceive::BeginState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToDiscard, bool(MachineH));
    MOCK_METHOD1(DiscardAction, void(MachineH));
    MOCK_METHOD1(ReceiveAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortReceive::BeginState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortReceive::BeginState::ChangeState(machine, newState);
    }
};

class DiscardState : public PortReceive::DiscardState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToReceive, bool(MachineH));
    MOCK_METHOD1(DiscardAction, void(MachineH));
    MOCK_METHOD1(ReceiveAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortReceive::DiscardState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortReceive::DiscardState::ChangeState(machine, newState);
    }
};

class ReceiveState : public PortReceive::ReceiveState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToReceive, bool(MachineH));
    MOCK_METHOD1(DiscardAction, void(MachineH));
    MOCK_METHOD1(ReceiveAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortReceive::ReceiveState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortReceive::ReceiveState::ChangeState(machine, newState);
    }
};

} // namespace Prx
} // namespace Mock

