/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {
namespace PortInformation {

class PimState : public State {
protected:
    __virtual void DisabledAction(Machine& machine);
    __virtual void AgedAction(Machine& machine);
    __virtual void UpdateAction(Machine& machine);
    __virtual void SuperiorDesignatedAction(Machine& machine);
    __virtual void RepeatedDesignatedAction(Machine& machine);
    __virtual void InferiorDesignatedAction(Machine& machine);
    __virtual void NotDesignatedAction(Machine& machine);
    __virtual void OtherAction(Machine& machine);
    __virtual void CurrentAction(Machine& machine);
    __virtual void ReceiveAction(Machine& machine);
    __virtual bool GoToCurrent(Machine& machine);
    __virtual void CurrentUctExecute(Machine& machine);
};

class BeginState : public PimState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToDisabled(Machine& machine);
    std::string Name() override;
};

class DisabledState : public PimState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DisabledState() = default;
    __virtual bool GoToDisabled(Machine& machine);
    __virtual bool GoToAged(Machine& machine);
    std::string Name() override;
};

class AgedState : public PimState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    AgedState() = default;
    __virtual bool GoToUpdate(Machine& machine);
    std::string Name() override;
};

class UpdateState : public PimState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    UpdateState() = default;
    std::string Name() override;
};

class SuperiorDesignatedState : public PimState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    SuperiorDesignatedState() = default;
    std::string Name() override;
};

class RepeatedDesignatedState : public PimState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    RepeatedDesignatedState() = default;
    std::string Name() override;
};

class InferiorDesignatedState : public PimState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    InferiorDesignatedState() = default;
    std::string Name() override;
};

class NotDesignatedState : public PimState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    NotDesignatedState() = default;
    std::string Name() override;
};

class OtherState : public PimState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    OtherState() = default;
    std::string Name() override;
};

class CurrentState : public PimState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    CurrentState() = default;
    __virtual bool GoToUpdate(Machine& machine);
    __virtual bool GoToAged(Machine& machine);
    __virtual bool GoToReceive(Machine& machine);
    std::string Name() override;
};

class ReceiveState : public PimState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    ReceiveState() = default;
    __virtual bool GoToSuperiorDesignated(Machine& machine);
    __virtual bool GoToRepeatedDesignated(Machine& machine);
    __virtual bool GoToInferiorDesignated(Machine& machine);
    __virtual bool GoToNotDesignated(Machine& machine);
    __virtual bool GoToOther(Machine& machine);
    std::string Name() override;
};

class PimMachine : public Machine {
public:
    PimMachine(BridgeH bridge, PortH port);
    std::string Name() override;
};

inline PimMachine::PimMachine(BridgeH bridge, PortH port)
    : Machine{ bridge, port, BeginState::Instance() } {
}

inline std::string PimMachine::Name() {
    return "PIM";
}

inline std::string BeginState::Name() {
    return "BEGIN";
}

inline std::string DisabledState::Name() {
    return "DISABLED";
}

inline std::string AgedState::Name() {
    return "AGED";
}

inline std::string UpdateState::Name() {
    return "UPDATE";
}

inline std::string SuperiorDesignatedState::Name() {
    return "SUPERIOR_DESIGNATED";
}

inline std::string RepeatedDesignatedState::Name() {
    return "REPEATED_DESIGNATED";
}

inline std::string InferiorDesignatedState::Name() {
    return "INFERIOR_DESIGNATED";
}

inline std::string NotDesignatedState::Name() {
    return "NOT_DESIGNATED";
}

inline std::string OtherState::Name() {
    return "OTHER";
}

inline std::string CurrentState::Name() {
    return "CURRENT";
}

inline std::string ReceiveState::Name() {
    return "RECEIVE";
}

} // namespace PortInformation
} // namespace Stp
