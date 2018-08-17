#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {
namespace PortInformation {

class PimState : public State {
protected:
    __virtual void DisabledAction(MachineH machine);
    __virtual void AgedAction(MachineH machine);
    __virtual void UpdateAction(MachineH machine);
    __virtual void SuperiorDesignatedAction(MachineH machine);
    __virtual void RepeatedDesignatedAction(MachineH machine);
    __virtual void InferiorDesignatedAction(MachineH machine);
    __virtual void NotDesignatedAction(MachineH machine);
    __virtual void OtherAction(MachineH machine);
    __virtual void CurrentAction(MachineH machine);
    __virtual void ReceiveAction(MachineH machine);
    __virtual bool GoToCurrent(MachineH machine);
    __virtual void CurrentUctExecute(MachineH machine);
};

class BeginState : public PimState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToDisabled(MachineH machine);
    std::string Name() override;
};

class DisabledState : public PimState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    DisabledState() = default;
    __virtual bool GoToDisabled(MachineH machine);
    __virtual bool GoToAged(MachineH machine);
    std::string Name() override;
};

class AgedState : public PimState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    AgedState() = default;
    __virtual bool GoToUpdate(MachineH machine);
    std::string Name() override;
};

class UpdateState : public PimState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    UpdateState() = default;
    std::string Name() override;
};

class SuperiorDesignatedState : public PimState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    SuperiorDesignatedState() = default;
    std::string Name() override;
};

class RepeatedDesignatedState : public PimState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    RepeatedDesignatedState() = default;
    std::string Name() override;
};

class InferiorDesignatedState : public PimState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    InferiorDesignatedState() = default;
    std::string Name() override;
};

class NotDesignatedState : public PimState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    NotDesignatedState() = default;
    std::string Name() override;
};

class OtherState : public PimState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    OtherState() = default;
    std::string Name() override;
};

class CurrentState : public PimState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    CurrentState() = default;
    __virtual bool GoToUpdate(MachineH machine);
    __virtual bool GoToAged(MachineH machine);
    __virtual bool GoToReceive(MachineH machine);
    std::string Name() override;
};

class ReceiveState : public PimState {
public:
    static StateH Instance();
    void Execute(MachineH machine) override;

protected:
    ReceiveState() = default;
    __virtual bool GoToSuperiorDesignated(MachineH machine);
    __virtual bool GoToRepeatedDesignated(MachineH machine);
    __virtual bool GoToInferiorDesignated(MachineH machine);
    __virtual bool GoToNotDesignated(MachineH machine);
    __virtual bool GoToOther(MachineH machine);
    std::string Name() override;
};

#define PIM_PREFIX_NAME "PIM @ "
constexpr auto kBeginStateName = PIM_PREFIX_NAME "BEGIN";
constexpr auto kDisabledStateName = PIM_PREFIX_NAME "DISABLED";
constexpr auto kAgedStateName = PIM_PREFIX_NAME "AGED";
constexpr auto kUpdateStateName = PIM_PREFIX_NAME "UPDATE";
constexpr auto kSuperiorDesignatedStateName = PIM_PREFIX_NAME "SUPERIOR_DESIGNATED";
constexpr auto kRepeatedDesignatedStateName = PIM_PREFIX_NAME "REPEATED_DESIGNATED";
constexpr auto kInferiorDesignatedStateName = PIM_PREFIX_NAME "INFERIOR_DESIGNATED";
constexpr auto kNotDesignatedStateName = PIM_PREFIX_NAME "NOT_DESIGNATED";
constexpr auto kOtherStateName = PIM_PREFIX_NAME "OTHER";
constexpr auto kCurrentStateName = PIM_PREFIX_NAME "CURRENT";
constexpr auto kReceiveStateName = PIM_PREFIX_NAME "RECEIVE";

inline std::string BeginState::Name() {
    return kBeginStateName;
}

inline std::string DisabledState::Name() {
    return kDisabledStateName;
}

inline std::string AgedState::Name() {
    return kAgedStateName;
}

inline std::string UpdateState::Name() {
    return kUpdateStateName;
}

inline std::string SuperiorDesignatedState::Name() {
    return kSuperiorDesignatedStateName;
}

inline std::string RepeatedDesignatedState::Name() {
    return kRepeatedDesignatedStateName;
}

inline std::string InferiorDesignatedState::Name() {
    return kInferiorDesignatedStateName;
}

inline std::string NotDesignatedState::Name() {
    return kNotDesignatedStateName;
}

inline std::string OtherState::Name() {
    return kOtherStateName;
}

inline std::string CurrentState::Name() {
    return kCurrentStateName;
}

inline std::string ReceiveState::Name() {
    return kReceiveStateName;
}

} // namespace PortInformation
} // namespace Stp
