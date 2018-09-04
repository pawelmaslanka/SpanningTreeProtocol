/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

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

class PrsMachine : public Machine {
public:
    PrsMachine(BridgeH bridge, PortH port);
    std::string Name() override;
};

inline PrsMachine::PrsMachine(BridgeH bridge, PortH port)
    : Machine{ bridge, port, BeginState::Instance() } {
}

inline std::string PrsMachine::Name() {
    return "PRS";
}

inline std::string BeginState::Name() {
    return "BEGIN";
}

inline std::string InitBridgeState::Name() {
    return "INIT_BRIDGE";
}

inline std::string RoleSelectionState::Name() {
    return "ROLE_SELECTION";
}

} // namespace PortRoleSelection
} // namespace Stp
