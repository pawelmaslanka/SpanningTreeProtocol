#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {
namespace PortRoleSelection {

class PrsState : public State {
protected:
    __virtual void InitBridgeAction(Machine& machine);
    __virtual void RoleSelectionAction(Machine& machine);
};

class BeginState : public PrsState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToInitBridge(Machine& machine);
    std::string Name() override;
};

class InitBridgeState : public PrsState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    InitBridgeState() = default;
    __virtual bool GoToRoleSelection(Machine& machine);
    std::string Name() override;
};

class RoleSelectionState : public PrsState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    RoleSelectionState() = default;
    __virtual bool GoToRoleSelection(Machine& machine);
    std::string Name() override;
};

#define PRS_PREFIX_NAME "PRS @ "
constexpr auto kBeginStateName = PRS_PREFIX_NAME "BEGIN";
constexpr auto kInitBridgeStateName = PRS_PREFIX_NAME "INIT_BRIDGE";
constexpr auto kRoleSelectionStateName = PRS_PREFIX_NAME "ROLE_SELECTION";

inline std::string BeginState::Name() {
    return kBeginStateName;
}

inline std::string InitBridgeState::Name() {
    return kInitBridgeStateName;
}

inline std::string RoleSelectionState::Name() {
    return kRoleSelectionStateName;
}

} // namespace PortTransmit
} // namespace Stp
