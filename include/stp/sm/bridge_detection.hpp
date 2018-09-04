/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

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

class BdmMachine : public Machine {
public:
    BdmMachine(BridgeH bridge, PortH port);
    std::string Name() override;
};

inline BdmMachine::BdmMachine(BridgeH bridge, PortH port)
    : Machine{ bridge, port, BeginState::Instance() } {
}

inline std::string BdmMachine::Name() {
    return "BDM";
}

inline std::string BeginState::Name() {
    return "BEGIN";
}

inline std::string EdgeState::Name() {
    return "EDGE";
}

inline std::string NotEdgeState::Name() {
    return "NOT_EDGE";
}

} // namespace BridgeDetection
} // namespace Stp
