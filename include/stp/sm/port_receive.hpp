/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {
namespace PortReceive {

/**
 * @brief Represents states of Port Receive State Machine.
 * @note State Class
 */
class PrxState : public State {
protected:
    __virtual void DiscardAction(Machine& machine);
    __virtual void ReceiveAction(Machine& machine);
};

class BeginState : public PrxState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToDiscard(Machine& machine);
    std::string Name() override;
};

class DiscardState : public PrxState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DiscardState() = default;
    __virtual bool GoToReceive(Machine& machine);
    std::string Name() override;
};

class ReceiveState : public PrxState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    ReceiveState() = default;
    __virtual bool GoToReceive(Machine& machine);
    std::string Name() override;
};

class PrxMachine : public Machine {
public:
    PrxMachine(BridgeH bridge, PortH port);
    std::string Name() override;
};

inline PrxMachine::PrxMachine(BridgeH bridge, PortH port)
    : Machine{ bridge, port, BeginState::Instance() } {
}

inline std::string PrxMachine::Name() {
    return "PRX";
}

inline std::string BeginState::Name() {
    return "BEGIN";
}

inline std::string DiscardState::Name() {
    return "DISCARD";
}

inline std::string ReceiveState::Name() {
    return "RECEIVE";
}

} // namespace PortReceive
} // namespace Stp
