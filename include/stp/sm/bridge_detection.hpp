#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {
namespace BridgeDetection {

class BdmState : public State {
protected:
    __virtual void EdgeAction(MachineH machine);
    __virtual void NotEdgeAction(MachineH machine);
};

class BeginState : public BdmState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToEdge(MachineH machine);
    std::string Name() override;
};

class EdgeState : public BdmState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    EdgeState() = default;
    __virtual bool GoToNotEdge(MachineH machine);
    std::string Name() override;
};

class NotEdgeState : public BdmState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    NotEdgeState() = default;
    __virtual bool GoToEdge(MachineH machine);
    std::string Name() override;
};

#define PRX_PREFIX_NAME "BDM @ "
constexpr auto kBeginStateName = PRX_PREFIX_NAME "BEGIN";
constexpr auto kEdgeStateName = PRX_PREFIX_NAME "EDGE";
constexpr auto kNotEdgeStateName = PRX_PREFIX_NAME "NOT_EDGE";

inline std::string BeginState::Name() {
    return kBeginStateName;
}

inline std::string EdgeState::Name() {
    return kEdgeStateName;
}

inline std::string NotEdgeState::Name() {
    return kNotEdgeStateName;
}

} // namespace BridgeDetection
} // namespace SpanningTree
