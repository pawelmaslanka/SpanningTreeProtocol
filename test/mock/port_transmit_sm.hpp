/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// Tested project's headers
#include <stp/sm/port_transmit.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {
namespace Ptx {

using namespace Stp;

class BeginState : public PortTransmit::BeginState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(TransmitInitAction, void(Machine&));
    MOCK_METHOD1(TransmitPeriodicAction, void(Machine&));
    MOCK_METHOD1(TransmitConfigAction, void(Machine&));
    MOCK_METHOD1(TransmitTcnAction, void(Machine&));
    MOCK_METHOD1(TransmitRstpAction, void(Machine&));
    MOCK_METHOD1(IdleAction, void(Machine&));
    MOCK_METHOD1(GoToTransmitInit, bool(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortTransmit::BeginState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortTransmit::BeginState::ChangeState(machine, newState);
    }
};

class TransmitInitState : public PortTransmit::TransmitInitState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(TransmitInitAction, void(Machine&));
    MOCK_METHOD1(TransmitPeriodicAction, void(Machine&));
    MOCK_METHOD1(TransmitConfigAction, void(Machine&));
    MOCK_METHOD1(TransmitTcnAction, void(Machine&));
    MOCK_METHOD1(TransmitRstpAction, void(Machine&));
    MOCK_METHOD1(IdleAction, void(Machine&));
    MOCK_METHOD1(GoToIdle, bool(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortTransmit::TransmitInitState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortTransmit::TransmitInitState::ChangeState(machine, newState);
    }
};

class TransmitPeriodicState : public PortTransmit::TransmitPeriodicState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(TransmitInitAction, void(Machine&));
    MOCK_METHOD1(TransmitPeriodicAction, void(Machine&));
    MOCK_METHOD1(TransmitConfigAction, void(Machine&));
    MOCK_METHOD1(TransmitTcnAction, void(Machine&));
    MOCK_METHOD1(TransmitRstpAction, void(Machine&));
    MOCK_METHOD1(IdleAction, void(Machine&));
    MOCK_METHOD1(GoToIdle, bool(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortTransmit::TransmitPeriodicState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortTransmit::TransmitPeriodicState::ChangeState(machine, newState);
    }
};

class TransmitConfigState : public PortTransmit::TransmitConfigState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(TransmitInitAction, void(Machine&));
    MOCK_METHOD1(TransmitPeriodicAction, void(Machine&));
    MOCK_METHOD1(TransmitConfigAction, void(Machine&));
    MOCK_METHOD1(TransmitTcnAction, void(Machine&));
    MOCK_METHOD1(TransmitRstpAction, void(Machine&));
    MOCK_METHOD1(IdleAction, void(Machine&));
    MOCK_METHOD1(GoToIdle, bool(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortTransmit::TransmitConfigState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortTransmit::TransmitConfigState::ChangeState(machine, newState);
    }
};

class TransmitTcnState : public PortTransmit::TransmitTcnState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(TransmitInitAction, void(Machine&));
    MOCK_METHOD1(TransmitPeriodicAction, void(Machine&));
    MOCK_METHOD1(TransmitConfigAction, void(Machine&));
    MOCK_METHOD1(TransmitTcnAction, void(Machine&));
    MOCK_METHOD1(TransmitRstpAction, void(Machine&));
    MOCK_METHOD1(IdleAction, void(Machine&));
    MOCK_METHOD1(GoToIdle, bool(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortTransmit::TransmitTcnState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortTransmit::TransmitTcnState::ChangeState(machine, newState);
    }
};

class TransmitRstpState : public PortTransmit::TransmitRstpState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(TransmitInitAction, void(Machine&));
    MOCK_METHOD1(TransmitPeriodicAction, void(Machine&));
    MOCK_METHOD1(TransmitConfigAction, void(Machine&));
    MOCK_METHOD1(TransmitTcnAction, void(Machine&));
    MOCK_METHOD1(TransmitRstpAction, void(Machine&));
    MOCK_METHOD1(IdleAction, void(Machine&));
    MOCK_METHOD1(GoToIdle, bool(Machine&));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortTransmit::TransmitRstpState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortTransmit::TransmitRstpState::ChangeState(machine, newState);
    }
};

class IdleState : public PortTransmit::IdleState {
public:
    MOCK_METHOD0(Instance, State&());
    MOCK_METHOD1(Execute, void(Machine&));
    MOCK_METHOD1(TransmitInitAction, void(Machine&));
    MOCK_METHOD1(TransmitPeriodicAction, void(Machine&));
    MOCK_METHOD1(TransmitConfigAction, void(Machine&));
    MOCK_METHOD1(TransmitTcnAction, void(Machine&));
    MOCK_METHOD1(TransmitRstpAction, void(Machine&));
    MOCK_METHOD1(IdleAction, void(Machine&));
    MOCK_METHOD1(GoToTransmitPeriodic, bool(Machine& machine));
    MOCK_METHOD1(GoToTransmitConfig, bool(Machine& machine));
    MOCK_METHOD1(GoToTransmitTcn, bool(Machine& machine));
    MOCK_METHOD1(GoToTransmitRstp, bool(Machine& machine));
    MOCK_METHOD2(ChangeState, void(Machine&, State&));

    void RealExecute(Machine& machine) {
        PortTransmit::IdleState::Execute(machine);
    }

    void RealChangeState(Machine& machine, State& newState) {
        PortTransmit::IdleState::ChangeState(machine, newState);
    }
};

} // namespace Ptx
} // namespace Stp
