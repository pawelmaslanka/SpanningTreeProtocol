#pragma once

// This project's headers
#include "bridge.hpp"
#include "port.hpp"
#include "specifiers.hpp"

// C++ Standard Library
#include <memory>
#include <string>

namespace SpanningTree {

class Machine;
using MachineH = Machine&;
class State;
using StateH = State&;

#define RETURN_STATE_SINGLETON_INSTANCE(DERIVED_STATE_CLASS)    \
    static DERIVED_STATE_CLASS instance;                        \
    return dynamic_cast<State&>(instance)

class State {
public:
    virtual void Execute(MachineH machine) = 0;
    virtual std::string Name() = 0;

protected:
    virtual ~State() = default;
    __virtual void ChangeState(MachineH machine, StateH newState);
};

class Machine {
public:
    explicit Machine(sptr<Bridge> bridge, sptr<Port> port, StateH initState);
    void Run();
    __virtual Bridge& BridgeInstance() const noexcept;
    Port& PortInstance() const noexcept;

protected:
    friend class State;
    /**
     * @brief ChangeState
     * @note We are protected from null pointer by passing reference
     * @param state
     */
    void ChangeState(StateH newState);
    StateH CurrentState() const noexcept;

private:
    /// @todo static member because all ports working on single Bridge instance
    sptr<Bridge> _bridge;
    sptr<Port> _port;
    State* _state;
};

//template <typename D>
//inline StateH State::SingletonInstance() {
//    static D instance { };

//    //    if (not std::is_base_of<B, D>::value) {
//    //        static B rescueInstance { };
//    //        return rescueInstance;
//    //    }

//    return dynamic_cast<StateH>(instance);
//}

inline void State::ChangeState(MachineH machine, StateH newState) {
    machine.ChangeState(newState);
}

inline void Machine::Run() {
    _state->Execute(*this);
}

inline BridgeH Machine::BridgeInstance() const noexcept {
    return *_bridge;
}

inline PortH Machine::PortInstance() const noexcept {
    return *_port;
}

inline void Machine::ChangeState(StateH newState) {
    _state = &newState;
}

inline StateH Machine::CurrentState() const noexcept {
    return *_state;
}

} // namespace SpanningTree
