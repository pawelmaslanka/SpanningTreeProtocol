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
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(DisabledAction, void(MachineH machine));
    MOCK_METHOD1(AgedAction, void(MachineH machine));
    MOCK_METHOD1(UpdateAction, void(MachineH machine));
    MOCK_METHOD1(SuperiorDesignatedAction, void(MachineH machine));
    MOCK_METHOD1(RepeatedDesignatedAction, void(MachineH machine));
    MOCK_METHOD1(InferiorDesignatedAction, void(MachineH machine));
    MOCK_METHOD1(NotDesignatedAction, void(MachineH machine));
    MOCK_METHOD1(OtherAction, void(MachineH machine));
    MOCK_METHOD1(CurrentAction, void(MachineH machine));
    MOCK_METHOD1(ReceiveAction, void(MachineH machine));
    MOCK_METHOD1(GoToDisabled, bool(MachineH machine));
    MOCK_METHOD1(GoToCurrent, bool(MachineH machine));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortInformation::BeginState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortInformation::BeginState::ChangeState(machine, newState);
    }
};

} // namespace Pim
} // namespace Mock
