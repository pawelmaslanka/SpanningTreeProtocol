/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {
namespace PortTransmit {

class PtxState : public State {
protected:
    __virtual void TransmitInitAction(Machine& machine);
    __virtual void TransmitPeriodicAction(Machine& machine);
    __virtual void TransmitConfigAction(Machine& machine);
    __virtual void TransmitTcnAction(Machine& machine);
    __virtual void TransmitRstpAction(Machine& machine);
    __virtual bool GoToIdle(Machine& machine);
    __virtual void IdleAction(Machine& machine);
    __virtual void IdleUctExecute(Machine& machine);
    __virtual bool TransitionQualified(Machine& machine);
};

class BeginState : public PtxState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToTransmitInit(Machine& machine);
    std::string Name() override;
};

class TransmitInitState : public PtxState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    TransmitInitState() = default;
    std::string Name() override;
};

class TransmitPeriodicState : public PtxState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    TransmitPeriodicState() = default;
    std::string Name() override;
};

class TransmitConfigState : public PtxState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    TransmitConfigState() = default;
    std::string Name() override;
};

class TransmitTcnState : public PtxState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    TransmitTcnState() = default;
    std::string Name() override;
};

class TransmitRstpState : public PtxState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    TransmitRstpState() = default;
    std::string Name() override;
};

class IdleState : public PtxState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    IdleState() = default;
    __virtual bool GoToTransmitPeriodic(Machine& machine);
    __virtual bool GoToTransmitConfig(Machine& machine);
    __virtual bool GoToTransmitTcn(Machine& machine);
    __virtual bool GoToTransmitRstp(Machine& machine);
    std::string Name() override;
};

class PtxMachine : public Machine {
public:
    PtxMachine(BridgeH bridge, PortH port);
    std::string Name() override;
};

inline PtxMachine::PtxMachine(BridgeH bridge, PortH port)
    : Machine{ bridge, port, BeginState::Instance() } {
}

inline std::string PtxMachine::Name() {
    return "PTX";
}

inline std::string BeginState::Name() {
    return "BEGIN";
}

inline std::string TransmitInitState::Name() {
    return "TRANSMIT_INIT";
}

inline std::string TransmitPeriodicState::Name() {
    return "TRANSMIT_PERIODIC";
}

inline std::string TransmitConfigState::Name() {
    return "TRANSMIT_CONFIG";
}

inline std::string TransmitTcnState::Name() {
    return "TRANSMIT_TCN";
}

inline std::string TransmitRstpState::Name() {
    return "TRANSMIT_RSTP";
}

inline std::string IdleState::Name() {
    return "IDLE";
}

} // namespace PortTransmit
} // namespace Stp
