/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

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
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToEdge, bool(Machine&));
    MOCK_METHOD1(EdgeAction, void(Machine&));
    MOCK_METHOD1(NotEdgeAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        BridgeDetection::BeginState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        BridgeDetection::BeginState::ChangeState(machine, newState);
    }
};

class EdgeState : public BridgeDetection::EdgeState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToNotEdge, bool(Machine&));
    MOCK_METHOD1(EdgeAction, void(Machine&));
    MOCK_METHOD1(NotEdgeAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        BridgeDetection::EdgeState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        BridgeDetection::EdgeState::ChangeState(machine, newState);
    }
};

class NotEdgeState : public BridgeDetection::NotEdgeState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToEdge, bool(Machine&));
    MOCK_METHOD1(EdgeAction, void(Machine&));
    MOCK_METHOD1(NotEdgeAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        BridgeDetection::NotEdgeState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        BridgeDetection::NotEdgeState::ChangeState(machine, newState);
    }
};

} // namespace Bdm
} // namespace Mock

