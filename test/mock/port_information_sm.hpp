/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// Tested project's headers
#include <stp/sm/port_information.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {
namespace Pim {

using namespace Stp;

class BeginState : public PortInformation::BeginState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(DisabledAction, void(Machine& machine));
    MOCK_METHOD1(AgedAction, void(Machine& machine));
    MOCK_METHOD1(UpdateAction, void(Machine& machine));
    MOCK_METHOD1(SuperiorDesignatedAction, void(Machine& machine));
    MOCK_METHOD1(RepeatedDesignatedAction, void(Machine& machine));
    MOCK_METHOD1(InferiorDesignatedAction, void(Machine& machine));
    MOCK_METHOD1(NotDesignatedAction, void(Machine& machine));
    MOCK_METHOD1(OtherAction, void(Machine& machine));
    MOCK_METHOD1(CurrentAction, void(Machine& machine));
    MOCK_METHOD1(ReceiveAction, void(Machine& machine));
    MOCK_METHOD1(GoToDisabled, bool(Machine& machine));
    MOCK_METHOD1(GoToCurrent, bool(Machine& machine));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortInformation::BeginState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortInformation::BeginState::ChangeState(machine, newState);
    }
};

} // namespace Pim
} // namespace Mock
