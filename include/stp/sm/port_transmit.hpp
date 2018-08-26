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

#define PTX_PREFIX_NAME "PTX @ "
constexpr auto kBeginStateName = PTX_PREFIX_NAME "BEGIN";
constexpr auto kTransmitInitStateName = PTX_PREFIX_NAME "TRANSMIT_INIT";
constexpr auto kTransmitPeriodicStateName = PTX_PREFIX_NAME "TRANSMIT_PERIODIC";
constexpr auto kTransmitConfigStateName = PTX_PREFIX_NAME "TRANSMIT_CONFIG";
constexpr auto kTransmitTcnStateName = PTX_PREFIX_NAME "TRANSMIT_TCN";
constexpr auto kTransmitRstpStateName = PTX_PREFIX_NAME "TRANSMIT_RSTP";
constexpr auto kIdleStateName = PTX_PREFIX_NAME "IDLE";

inline std::string BeginState::Name() {
    return kBeginStateName;
}

inline std::string TransmitInitState::Name() {
    return kTransmitInitStateName;
}

inline std::string TransmitPeriodicState::Name() {
    return kTransmitPeriodicStateName;
}

inline std::string TransmitConfigState::Name() {
    return kTransmitConfigStateName;
}

inline std::string TransmitTcnState::Name() {
    return kTransmitTcnStateName;
}

inline std::string TransmitRstpState::Name() {
    return kTransmitRstpStateName;
}

inline std::string IdleState::Name() {
    return kIdleStateName;
}

} // namespace PortTransmit
} // namespace Stp
