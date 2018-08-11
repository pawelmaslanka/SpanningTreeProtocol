#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace SpanningTree {
namespace PortReceive {

/**
 * @brief Represents states of Port Receive State Machine.
 * @note State Class
 */
class PrxState : public State {
protected:
    __virtual void DiscardAction(MachineH machine);
    __virtual void ReceiveAction(MachineH machine);
};

class BeginState : public PrxState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToDiscard(MachineH machine);
    std::string Name() override;
};

class DiscardState : public PrxState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    DiscardState() = default;
    __virtual bool GoToReceive(MachineH machine);
    std::string Name() override;
};

class ReceiveState : public PrxState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    ReceiveState() = default;
    __virtual bool GoToReceive(MachineH machine);
    std::string Name() override;
};

#define PRX_PREFIX_NAME "PRX @ "
constexpr auto kBeginStateName = PRX_PREFIX_NAME "BEGIN";
constexpr auto kDiscardStateName = PRX_PREFIX_NAME "DISCARD";
constexpr auto kReceiveStateName = PRX_PREFIX_NAME "RECEIVE";

inline std::string BeginState::Name() {
    return kBeginStateName;
}

inline std::string DiscardState::Name() {
    return kDiscardStateName;
}

inline std::string ReceiveState::Name() {
    return kReceiveStateName;
}

} // namespace PortReceive
} // namespace SpanningTree
