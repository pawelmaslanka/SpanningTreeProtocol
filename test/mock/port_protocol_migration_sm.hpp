/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// Tested project's headers
#include <stp/sm/port_protocol_migration.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {
using namespace Stp;

namespace Ppm {

class BeginState : public PortProtocolMigration::BeginState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToCheckingRstp, bool(Machine&));
    MOCK_METHOD1(BeginAction, void(Machine&));
    MOCK_METHOD1(CheckingRstpAction, void(Machine&));
    MOCK_METHOD1(SelectingStpAction, void(Machine&));
    MOCK_METHOD1(SensingAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortProtocolMigration::BeginState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortProtocolMigration::BeginState::ChangeState(machine, newState);
    }
};

class CheckingRstpState : public PortProtocolMigration::CheckingRstpState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToSensing, bool(Machine&));
    MOCK_METHOD1(GoToCheckingRstp, bool(Machine&));
    MOCK_METHOD1(BeginAction, void(Machine&));
    MOCK_METHOD1(CheckingRstpAction, void(Machine&));
    MOCK_METHOD1(SelectingStpAction, void(Machine&));
    MOCK_METHOD1(SensingAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortProtocolMigration::CheckingRstpState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortProtocolMigration::CheckingRstpState::ChangeState(machine, newState);
    }
};

class SensingState : public PortProtocolMigration::SensingState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToCheckingRstp, bool(Machine&));
    MOCK_METHOD1(GoToSelectingStp, bool(Machine&));
    MOCK_METHOD1(BeginAction, void(Machine&));
    MOCK_METHOD1(CheckingRstpAction, void(Machine&));
    MOCK_METHOD1(SelectingStpAction, void(Machine&));
    MOCK_METHOD1(SensingAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortProtocolMigration::SensingState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortProtocolMigration::SensingState::ChangeState(machine, newState);
    }
};

class SelectingStpState : public PortProtocolMigration::SelectingStpState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(GoToSensing, bool(Machine&));
    MOCK_METHOD1(BeginAction, void(Machine&));
    MOCK_METHOD1(CheckingRstpAction, void(Machine&));
    MOCK_METHOD1(SelectingStpAction, void(Machine&));
    MOCK_METHOD1(SensingAction, void(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortProtocolMigration::SelectingStpState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortProtocolMigration::SelectingStpState::ChangeState(machine, newState);
    }
};

} // namespace Ppm
} // namespace Mock
