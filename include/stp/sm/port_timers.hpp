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

#define PTI_PREFIX_NAME "PTI @ "
constexpr auto kBeginStateName = PTI_PREFIX_NAME "BEGIN";
constexpr auto kOneSecondStateName = PTI_PREFIX_NAME "ONE_SECOND";
constexpr auto kTickStateName = PTI_PREFIX_NAME "TICK";

inline std::string BeginState::Name() {
    return kBeginStateName;
}

inline std::string OneSecondState::Name() {
    return kOneSecondStateName;
}

inline std::string TickState::Name() {
    return kTickStateName;
}

} // namespace PortTimers
} // namespace Stp
