/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

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

class PstMachine : public Machine {
public:
    PstMachine(BridgeH bridge, PortH port);
    std::string Name() override;
};

inline PstMachine::PstMachine(BridgeH bridge, PortH port)
    : Machine{ bridge, port, BeginState::Instance() } {
}

inline std::string Name() {
    return "PST";
}

inline std::string BeginState::Name() {
    return "BEGIN";
}

inline std::string DiscardingState::Name() {
    return "DISCARDING";
}

inline std::string LearningState::Name() {
    return "LEARNING";
}

inline std::string ForwardingState::Name() {
    return "FORWARDING";
}

} // namespace PortStateTransition
} // namespace Stp
