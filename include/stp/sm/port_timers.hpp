/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {
namespace PortTimers {

class PtiTimers : public State {
protected:
    __virtual void OneSecondAction(Machine& machine);
    __virtual void TickAction(Machine& machine);
};

class BeginState : public PtiTimers {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToOneSecond(Machine& machine);
    std::string Name() override;
};

class OneSecondState : public PtiTimers {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    OneSecondState() = default;
    __virtual bool GoToTick(Machine& machine);
    std::string Name() override;
};

class TickState : public PtiTimers {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    TickState() = default;
    __virtual bool GoToOneSecond(Machine& machine);
    std::string Name() override;
};

class PtiMachine : public Machine {
public:
    PtiMachine(BridgeH bridge, PortH port);
    std::string Name() override;
};

inline PtiMachine::PtiMachine(BridgeH bridge, PortH port)
    : Machine{ bridge, port, BeginState::Instance() } {
}

inline std::string PtiMachine::Name() {
    return "PTI";
}

inline std::string BeginState::Name() {
    return "BEGIN";
}

inline std::string OneSecondState::Name() {
    return "ONE_SECOND";
}

inline std::string TickState::Name() {
    return "TICK";
}

} // namespace PortTimers
} // namespace Stp
