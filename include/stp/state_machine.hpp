/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "bridge.hpp"
#include "port.hpp"
#include "specifiers.hpp"

// C++ Standard Library
#include <memory>
#include <string>

namespace Stp {

class Machine;

#define RETURN_STATE_SINGLETON_INSTANCE(DERIVED_STATE_CLASS)    \
    static DERIVED_STATE_CLASS instance;                        \
    return dynamic_cast<State&>(instance)

class State {
public:
    virtual void Execute(Machine& machine) = 0;
    virtual std::string Name() = 0;

protected:
    virtual ~State() = default;
    __virtual void ChangeState(Machine& machine, State& newState);
};

class Machine {
public:
    explicit Machine(BridgeH bridge, PortH port, State& initState);
    void Run();
    virtual std::string Name() { return std::string{}; }
    __virtual Bridge& BridgeInstance() const noexcept;
    Port& PortInstance() const noexcept;

protected:
    friend class State;
    /**
     * @brief ChangeState
     * @note We are protected from null pointer by passing reference
     * @param state
     */
    void ChangeState(State& newState);
    State& CurrentState() const noexcept;

private:
    /// @todo static member because all ports working on single Bridge instance
    BridgeH _bridge;
    PortH _port;
    State* _state;
};

inline void Machine::Run() {
    _state->Execute(*this);
}

inline Bridge& Machine::BridgeInstance() const noexcept {
    return *_bridge;
}

inline Port& Machine::PortInstance() const noexcept {
    return *_port;
}

inline void Machine::ChangeState(State& newState) {
    _state = &newState;
}

inline State& Machine::CurrentState() const noexcept {
    return *_state;
}

} // namespace Stp
