#pragma once

// Tested project's headers
#include <stp/sm/bridge_detection.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {
namespace Bdm {

using namespace Stp;

class BeginState : public BridgeDetection::BeginState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToEdge, bool(MachineH));
    MOCK_METHOD1(EdgeAction, void(MachineH));
    MOCK_METHOD1(NotEdgeAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        BridgeDetection::BeginState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        BridgeDetection::BeginState::ChangeState(machine, newState);
    }
};

class EdgeState : public BridgeDetection::EdgeState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToNotEdge, bool(MachineH));
    MOCK_METHOD1(EdgeAction, void(MachineH));
    MOCK_METHOD1(NotEdgeAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        BridgeDetection::EdgeState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        BridgeDetection::EdgeState::ChangeState(machine, newState);
    }
};

class NotEdgeState : public BridgeDetection::NotEdgeState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToEdge, bool(MachineH));
    MOCK_METHOD1(EdgeAction, void(MachineH));
    MOCK_METHOD1(NotEdgeAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        BridgeDetection::NotEdgeState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        BridgeDetection::NotEdgeState::ChangeState(machine, newState);
    }
};

} // namespace Bdm
} // namespace Mock

