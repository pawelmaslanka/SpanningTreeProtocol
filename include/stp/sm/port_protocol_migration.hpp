/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "stp/bridge.hpp"
#include "stp/specifiers.hpp"
#include "stp/state_machine.hpp"
#include "stp/port.hpp"

// C++ Standard Library
#include <memory>

namespace Stp {
namespace PortProtocolMigration {

class PpmState : public State {
protected:
    __virtual void CheckingRstpAction(Machine& machine);
    __virtual void SelectingStpAction(Machine& machine);
    __virtual void SensingAction(Machine& machine);
};

class BeginState : public PpmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToCheckingRstp(Machine& machine);
    std::string Name() override;
};

class CheckingRstpState : public PpmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    CheckingRstpState() = default;
    __virtual bool GoToCheckingRstp(Machine& machine);
    __virtual bool GoToSensing(Machine& machine);
    std::string Name() override;
};

class SensingState : public PpmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    SensingState() = default;
    __virtual bool GoToCheckingRstp(Machine& machine);
    __virtual bool GoToSelectingStp(Machine& machine);
    std::string Name() override;
};

class SelectingStpState : public PpmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    SelectingStpState() = default;
    __virtual bool GoToSensing(Machine& machine);
    std::string Name() override;
};

class PpmMachine : public Machine {
public:
    PpmMachine(BridgeH bridge, PortH port);
    std::string Name() override;
};

inline PpmMachine::PpmMachine(BridgeH bridge, PortH port)
    : Machine{ bridge, port, BeginState::Instance() } {
}

inline std::string PpmMachine::Name() {
    return "PPM";
}

inline std::string BeginState::Name() {
    return "BEGIN";
}

inline std::string CheckingRstpState::Name() {
    return "CHECKING_RSTP";
}

inline std::string SelectingStpState::Name() {
    return "SELECTING_STP";
}

inline std::string SensingState::Name() {
    return "SENSING";
}

} // namespace PortProtocolMigration
} // namespace Stp
