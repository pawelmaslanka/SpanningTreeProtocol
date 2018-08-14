#pragma once

// Tested project's headers
#include <stp/sm/port_protocol_migration.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {
using namespace SpanningTree;

namespace Ppm {

class BeginState : public PortProtocolMigration::BeginState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToCheckingRstp, bool(MachineH));
    MOCK_METHOD1(BeginAction, void(MachineH));
    MOCK_METHOD1(CheckingRstpAction, void(MachineH));
    MOCK_METHOD1(SelectingStpAction, void(MachineH));
    MOCK_METHOD1(SensingAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortProtocolMigration::BeginState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortProtocolMigration::BeginState::ChangeState(machine, newState);
    }
};

class CheckingRstpState : public PortProtocolMigration::CheckingRstpState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToSensing, bool(MachineH));
    MOCK_METHOD1(GoToCheckingRstp, bool(MachineH));
    MOCK_METHOD1(BeginAction, void(MachineH));
    MOCK_METHOD1(CheckingRstpAction, void(MachineH));
    MOCK_METHOD1(SelectingStpAction, void(MachineH));
    MOCK_METHOD1(SensingAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortProtocolMigration::CheckingRstpState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortProtocolMigration::CheckingRstpState::ChangeState(machine, newState);
    }
};

class SensingState : public PortProtocolMigration::SensingState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToCheckingRstp, bool(MachineH));
    MOCK_METHOD1(GoToSelectingStp, bool(MachineH));
    MOCK_METHOD1(BeginAction, void(MachineH));
    MOCK_METHOD1(CheckingRstpAction, void(MachineH));
    MOCK_METHOD1(SelectingStpAction, void(MachineH));
    MOCK_METHOD1(SensingAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortProtocolMigration::SensingState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortProtocolMigration::SensingState::ChangeState(machine, newState);
    }
};

class SelectingStpState : public PortProtocolMigration::SelectingStpState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(GoToSensing, bool(MachineH));
    MOCK_METHOD1(BeginAction, void(MachineH));
    MOCK_METHOD1(CheckingRstpAction, void(MachineH));
    MOCK_METHOD1(SelectingStpAction, void(MachineH));
    MOCK_METHOD1(SensingAction, void(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortProtocolMigration::SelectingStpState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortProtocolMigration::SelectingStpState::ChangeState(machine, newState);
    }
};

} // namespace Ppm
} // namespace Mock
