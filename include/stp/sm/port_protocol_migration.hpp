#pragma once

// This project's headers
#include "stp/bridge.hpp"
#include "stp/specifiers.hpp"
#include "stp/state_machine.hpp"
#include "stp/port.hpp"
#include "stp/utils.hpp"

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

#define PPM_PREFIX_NAME "PPM @ "
constexpr auto kBeginStateName = PPM_PREFIX_NAME "BEGIN";
constexpr auto kCheckingRstpStateName = PPM_PREFIX_NAME "CHECKING_RSTP";
constexpr auto kSelectingStpStateName = PPM_PREFIX_NAME "SELECTING_STP";
constexpr auto kSensingStateName = PPM_PREFIX_NAME "SENSING";

inline std::string BeginState::Name() {
    return kBeginStateName;
}

inline std::string CheckingRstpState::Name() {
    return kCheckingRstpStateName;
}

inline std::string SelectingStpState::Name() {
    return kSelectingStpStateName;
}

inline std::string SensingState::Name() {
    return kSensingStateName;
}

} // namespace PortProtocolMigration
} // namespace Stp
