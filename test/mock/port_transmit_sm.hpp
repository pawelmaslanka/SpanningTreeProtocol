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
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(TransmitInitAction, void(MachineH));
    MOCK_METHOD1(TransmitPeriodicAction, void(MachineH));
    MOCK_METHOD1(TransmitConfigAction, void(MachineH));
    MOCK_METHOD1(TransmitTcnAction, void(MachineH));
    MOCK_METHOD1(TransmitRstpAction, void(MachineH));
    MOCK_METHOD1(IdleAction, void(MachineH));
    MOCK_METHOD1(GoToTransmitInit, bool(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortTransmit::BeginState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortTransmit::BeginState::ChangeState(machine, newState);
    }
};

class TransmitInitState : public PortTransmit::TransmitInitState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(TransmitInitAction, void(MachineH));
    MOCK_METHOD1(TransmitPeriodicAction, void(MachineH));
    MOCK_METHOD1(TransmitConfigAction, void(MachineH));
    MOCK_METHOD1(TransmitTcnAction, void(MachineH));
    MOCK_METHOD1(TransmitRstpAction, void(MachineH));
    MOCK_METHOD1(IdleAction, void(MachineH));
    MOCK_METHOD1(GoToIdle, bool(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortTransmit::TransmitInitState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortTransmit::TransmitInitState::ChangeState(machine, newState);
    }
};

class TransmitPeriodicState : public PortTransmit::TransmitPeriodicState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(TransmitInitAction, void(MachineH));
    MOCK_METHOD1(TransmitPeriodicAction, void(MachineH));
    MOCK_METHOD1(TransmitConfigAction, void(MachineH));
    MOCK_METHOD1(TransmitTcnAction, void(MachineH));
    MOCK_METHOD1(TransmitRstpAction, void(MachineH));
    MOCK_METHOD1(IdleAction, void(MachineH));
    MOCK_METHOD1(GoToIdle, bool(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortTransmit::TransmitPeriodicState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortTransmit::TransmitPeriodicState::ChangeState(machine, newState);
    }
};

class TransmitConfigState : public PortTransmit::TransmitConfigState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(TransmitInitAction, void(MachineH));
    MOCK_METHOD1(TransmitPeriodicAction, void(MachineH));
    MOCK_METHOD1(TransmitConfigAction, void(MachineH));
    MOCK_METHOD1(TransmitTcnAction, void(MachineH));
    MOCK_METHOD1(TransmitRstpAction, void(MachineH));
    MOCK_METHOD1(IdleAction, void(MachineH));
    MOCK_METHOD1(GoToIdle, bool(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortTransmit::TransmitConfigState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortTransmit::TransmitConfigState::ChangeState(machine, newState);
    }
};

class TransmitTcnState : public PortTransmit::TransmitTcnState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(TransmitInitAction, void(MachineH));
    MOCK_METHOD1(TransmitPeriodicAction, void(MachineH));
    MOCK_METHOD1(TransmitConfigAction, void(MachineH));
    MOCK_METHOD1(TransmitTcnAction, void(MachineH));
    MOCK_METHOD1(TransmitRstpAction, void(MachineH));
    MOCK_METHOD1(IdleAction, void(MachineH));
    MOCK_METHOD1(GoToIdle, bool(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortTransmit::TransmitTcnState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortTransmit::TransmitTcnState::ChangeState(machine, newState);
    }
};

class TransmitRstpState : public PortTransmit::TransmitRstpState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(TransmitInitAction, void(MachineH));
    MOCK_METHOD1(TransmitPeriodicAction, void(MachineH));
    MOCK_METHOD1(TransmitConfigAction, void(MachineH));
    MOCK_METHOD1(TransmitTcnAction, void(MachineH));
    MOCK_METHOD1(TransmitRstpAction, void(MachineH));
    MOCK_METHOD1(IdleAction, void(MachineH));
    MOCK_METHOD1(GoToIdle, bool(MachineH));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortTransmit::TransmitRstpState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortTransmit::TransmitRstpState::ChangeState(machine, newState);
    }
};

class IdleState : public PortTransmit::IdleState {
public:
    MOCK_METHOD0(Instance, StateH());
    MOCK_METHOD1(Execute, void(MachineH));
    MOCK_METHOD1(TransmitInitAction, void(MachineH));
    MOCK_METHOD1(TransmitPeriodicAction, void(MachineH));
    MOCK_METHOD1(TransmitConfigAction, void(MachineH));
    MOCK_METHOD1(TransmitTcnAction, void(MachineH));
    MOCK_METHOD1(TransmitRstpAction, void(MachineH));
    MOCK_METHOD1(IdleAction, void(MachineH));
    MOCK_METHOD1(GoToTransmitPeriodic, bool(MachineH machine));
    MOCK_METHOD1(GoToTransmitConfig, bool(MachineH machine));
    MOCK_METHOD1(GoToTransmitTcn, bool(MachineH machine));
    MOCK_METHOD1(GoToTransmitRstp, bool(MachineH machine));
    MOCK_METHOD2(ChangeState, void(MachineH, StateH));

    void RealExecute(MachineH machine) {
        PortTransmit::IdleState::Execute(machine);
    }

    void RealChangeState(MachineH machine, StateH newState) {
        PortTransmit::IdleState::ChangeState(machine, newState);
    }
};

} // namespace Ptx
} // namespace Stp
