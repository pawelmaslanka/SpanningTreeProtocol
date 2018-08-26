#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {
namespace BridgeDetection {

class BdmState : public State {
protected:
    __virtual void EdgeAction(Machine& machine);
    __virtual void NotEdgeAction(Machine& machine);
};

class BeginState : public BdmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToEdge(Machine& machine);
    std::string Name() override;
};

class EdgeState : public BdmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    EdgeState() = default;
    __virtual bool GoToNotEdge(Machine& machine);
    std::string Name() override;
};

class NotEdgeState : public BdmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    NotEdgeState() = default;
    __virtual bool GoToEdge(Machine& machine);
    std::string Name() override;
};

#define BDM_PREFIX_NAME "BDM @ "
constexpr auto kBeginStateName = BDM_PREFIX_NAME "BEGIN";
constexpr auto kEdgeStateName = BDM_PREFIX_NAME "EDGE";
constexpr auto kNotEdgeStateName = BDM_PREFIX_NAME "NOT_EDGE";

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
} // namespace Stp
