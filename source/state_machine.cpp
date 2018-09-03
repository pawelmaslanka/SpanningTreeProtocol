/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {

void State::ChangeState(Machine& machine, State& newState) {
    machine.BridgeInstance().SystemLogChangeState(machine.Name(), Name(), newState.Name());
    machine.ChangeState(newState);
}

Machine::Machine(BridgeH bridge, PortH port, State& initState)
    : _bridge{ bridge }, _port{ port }, _state{ &initState } {
    if (nullptr == _bridge) {
        std::runtime_error("Handler for bridge instance is null pointer");
    }

    if (nullptr == _port) {
        std::runtime_error("Handler for port instance is null pointer");
    }

    if (nullptr == _state) {
        std::runtime_error("Handler for initial state instance is null pointer");
    }
}

} // namespace Stp
