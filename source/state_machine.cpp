#include "stp/state_machine.hpp"

namespace Stp {

Machine::Machine(sptr<Bridge> bridge, sptr<Port> port, StateH initState)
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

} // namespace SpanningTree
