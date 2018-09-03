/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {
namespace TopologyChange {

class TcmState : public State {
protected:
    __virtual void InactiveAction(Machine& machine);
    __virtual void LearningAction(Machine& machine);
    __virtual void DetectedAction(Machine& machine);
    __virtual void NotifiedTcnAction(Machine& machine);
    __virtual void NotifiedTcAction(Machine& machine);
    __virtual void PropagatingAction(Machine& machine);
    __virtual void AcknowledgeAction(Machine& machine);
    __virtual void ActiveAction(Machine& machine);
    __virtual void ActiveUctExecute(Machine& machine);
};

class BeginState : public TcmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToInactive(Machine& machine);
    std::string Name() override;
};

class InactiveState : public TcmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    InactiveState() = default;
    __virtual bool GoToLearning(Machine& machine);
    std::string Name() override;
};

class LearningState : public TcmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    LearningState() = default;
    __virtual bool GoToDetected(Machine& machine);
    __virtual bool GoToInactive(Machine& machine);
    __virtual bool GoToLearning(Machine& machine);
    std::string Name() override;
};

class DetectedState : public TcmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DetectedState() = default;
    std::string Name() override;
};

class NotifiedTcnState : public TcmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    NotifiedTcnState() = default;
    __virtual bool GoToNotifiedTc(Machine& machine);
    std::string Name() override;
};

class NotifiedTcState : public TcmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    NotifiedTcState() = default;
    std::string Name() override;
};

class PropagatingState : public TcmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    PropagatingState() = default;
    std::string Name() override;
};

class AcknowledgedState : public TcmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    AcknowledgedState() = default;
    std::string Name() override;
};

class ActiveState : public TcmState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    ActiveState() = default;
    __virtual bool GoToLearning(Machine& machine);
    __virtual bool GoToNotifiedTcn(Machine& machine);
    __virtual bool GoToNotifiedTc(Machine& machine);
    __virtual bool GoToPropagating(Machine& machine);
    __virtual bool GoToAcknowledged(Machine& machine);
    std::string Name() override;
};

class TcmMachine : public Machine {
public:
    TcmMachine(BridgeH bridge, PortH port);
    std::string Name() override;
};

inline TcmMachine::TcmMachine(BridgeH bridge, PortH port)
    : Machine{ bridge, port, BeginState::Instance() } {
}

inline std::string Name() {
    return "TCM";
}

inline std::string BeginState::Name() {
    return "BEGIN";
}

inline std::string InactiveState::Name() {
    return "INACTIVE";
}

inline std::string LearningState::Name() {
    return "LEARNING";
}

inline std::string DetectedState::Name() {
    return "DETECTED";
}

inline std::string NotifiedTcnState::Name() {
    return "NOTIFIED_TCN";
}

inline std::string NotifiedTcState::Name() {
    return "NOTIFIED_TC";
}

inline std::string PropagatingState::Name() {
    return "PROPAGATING";
}

inline std::string AcknowledgedState::Name() {
    return "ACKNOWLEDGED";
}

inline std::string ActiveState::Name() {
    return "ACTIVE";
}

} // namespace TopologyChange
} // namespace Stp
