#pragma once

// This project's headers
#include "bridge.hpp"
#include "specifiers.hpp"
#include "port.hpp"
#include "utils.hpp"

// C++ Standard Library
#include <memory>

namespace SpanningTree {
namespace PortProtocolMigration {

class Machine;
class State;
using MachineH = Machine&;
using StateH = State&;

/**
 * @brief Represents states of Port Receive State Machine.
 * @note State Class
 */
class State {
public:
    virtual ~State() = default;
    virtual void Execute(MachineH machine) = 0;

protected:
    /**
     * @tparam D Derived class
     * @exception std::bad_cast if D is out of State class inheritance hierarchy
     */
    template <typename D>
    static StateH SingletonInstance();

    __virtual void BeginAction(MachineH machine);
    __virtual void CheckingRstpAction(MachineH machine);
    __virtual void SelectingStpAction(MachineH machine);
    __virtual void SensingAction(MachineH machine);
    __virtual void ChangeState(MachineH machine, StateH newState);
};

class BeginState : public State {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    friend class State;
    BeginState() = default;
    __virtual bool GoToCheckingRstp(MachineH machine);
};

class CheckingRstpState : public State {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    friend class State;
    CheckingRstpState() = default;
    __virtual bool GoToCheckingRstp(MachineH machine);
    __virtual bool GoToSensing(MachineH machine);
};

class SensingState : public State {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    friend class State;
    SensingState() = default;
    __virtual bool GoToCheckingRstp(MachineH machine);
    __virtual bool GoToSelectingStp(MachineH machine);
};

class SelectingStpState : public State {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    friend class State;
    SelectingStpState() = default;
    __virtual bool GoToSensing(MachineH machine);
};

class Machine {
public:
    explicit Machine(BridgeH bridge, PortH port);
    void Run();
    BridgeH BridgeInstance() const noexcept;
    PortH PortInstance() const noexcept;

protected:
    friend class State;
    /**
     * @brief ChangeState
     * @note We are protected from null pointer by passing reference
     * @param state
     */
    void ChangeState(StateH newState);

private:
    BridgeH _bridge;
    PortH _port;
    State* _state;
};

template <typename D>
StateH State::SingletonInstance() {
    static D instance { };

    //    if (not std::is_base_of<B, D>::value) {
    //        static B rescueInstance { };
    //        return rescueInstance;
    //    }

    return dynamic_cast<State&>(instance);
}

inline void State::BeginAction(MachineH machine) {
    std::ignore = machine;
    // Nothing more to do
}

inline void State::ChangeState(MachineH machine, StateH newState) {
    machine.ChangeState(newState);
}

inline StateH BeginState::Instance() {
    return State::SingletonInstance<BeginState>();
}

inline StateH CheckingRstpState::Instance() {
    return State::SingletonInstance<CheckingRstpState>();
}

inline StateH SensingState::Instance() {
    return State::SingletonInstance<SensingState>();
}

inline StateH SelectingStpState::Instance() {
    return State::SingletonInstance<SelectingStpState>();
}

inline Machine::Machine(BridgeH bridge, PortH port)
    : _bridge{ bridge }, _port{ port }, _state{ &BeginState::Instance() } {
    // Nothing more to do
}

inline void Machine::Run() {
    _state->Execute(*this);
}

inline BridgeH Machine::BridgeInstance() const noexcept {
    return _bridge;
}

inline PortH Machine::PortInstance() const noexcept {
    return _port;
}

inline void Machine::ChangeState(StateH newState) {
    _state = &newState;
}

} // namespace PortProtocolMigration
} // namespace SpanningTree
