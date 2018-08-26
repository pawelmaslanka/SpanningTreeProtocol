#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {
namespace PortStateTransition {

class PstState : public State {
protected:
    __virtual void DiscardingAction(Machine& machine);
    __virtual void LearningAction(Machine& machine);
    __virtual void ForwardingAction(Machine& machine);
};

class BeginState : public PstState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToDiscarding(Machine& machine);
    std::string Name() override;
};

class DiscardingState : public PstState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DiscardingState() = default;
    __virtual bool GoToLearning(Machine& machine);
    std::string Name() override;
};

class LearningState : public PstState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    LearningState() = default;
    __virtual bool GoToForwarding(Machine& machine);
    __virtual bool GoToDiscarding(Machine& machine);
    std::string Name() override;
};

class ForwardingState : public PstState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    ForwardingState() = default;
    __virtual bool GoToDiscarding(Machine& machine);
    std::string Name() override;
};

#define PST_PREFIX_NAME "PST @ "
constexpr auto kBeginStateName = PST_PREFIX_NAME "BEGIN";
constexpr auto kDiscardingStateName = PST_PREFIX_NAME "DISCARDING";
constexpr auto kLearningStateName = PST_PREFIX_NAME "LEARNING";
constexpr auto kForwardingStateName = PST_PREFIX_NAME "FORWARDING";

inline std::string BeginState::Name() {
    return kBeginStateName;
}

inline std::string DiscardingState::Name() {
    return kDiscardingStateName;
}

inline std::string LearningState::Name() {
    return kLearningStateName;
}

inline std::string ForwardingState::Name() {
    return kForwardingStateName;
}

} // namespace PortStateTransition
} // namespace Stp
