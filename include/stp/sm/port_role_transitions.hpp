/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "stp/state_machine.hpp"

namespace Stp {
namespace PortRoleTransitions {

class PrtState : public State {
protected:
    // Disabled Port States's actions
    __virtual void InitPortAction(Machine& machine);
    __virtual void DisablePortAction(Machine& machine);
    __virtual void DisabledPortAction(Machine& machine);
    // Root Port States's actions
    __virtual void RootProposedAction(Machine& machine);
    __virtual void RootAgreedAction(Machine& machine);
    __virtual void ReRootAction(Machine& machine);
    __virtual void RootForwardAction(Machine& machine);
    __virtual void RootLearnAction(Machine& machine);
    __virtual void ReRootedAction(Machine& machine);
    __virtual void RootPortAction(Machine& machine);
    // Designated Port States's actions
    __virtual void DesignatedProposeAction(Machine& machine);
    __virtual void DesignatedSyncedAction(Machine& machine);
    __virtual void DesignatedRetiredAction(Machine& machine);
    __virtual void DesignatedForwardAction(Machine& machine);
    __virtual void DesignatedLearnAction(Machine& machine);
    __virtual void DesignatedDiscardAction(Machine& machine);
    __virtual void DesignatedPortAction(Machine& machine);
    // Alternate and Backup Port States's actions
    __virtual void AlternateProposedAction(Machine& machine);
    __virtual void AlternateAgreedAction(Machine& machine);
    __virtual void BlockPortAction(Machine& machine);
    __virtual void BackupPortAction(Machine& machine);
    __virtual void AlternatePortAction(Machine& machine);

    __virtual void RootPortUctExecute(Machine& machine);
    __virtual void DesignatedPortUctExecute(Machine& machine);
    __virtual void AlternatePortUctExecute(Machine& machine);

    __virtual bool ContinueExecute(Machine& machine);
    __virtual bool TransitionQualified(Machine& machine);
};

class BeginState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BeginState() = default;
    __virtual bool GoToInitPort(Machine& machine);
    std::string Name() override;
};

class InitPortState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    InitPortState() = default;
    __virtual bool GoToDisablePort(Machine& machine);
    std::string Name() override;
};

class DisablePortState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DisablePortState() = default;
    __virtual bool GoToDisabledPort(Machine& machine);
    std::string Name() override;
};

class DisabledPortState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DisabledPortState() = default;
    __virtual bool GoToDisabledPort(Machine& machine);
    std::string Name() override;
};

// Root Port States
class RootProposedState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    RootProposedState() = default;
    __virtual bool GoToRootPort(Machine& machine);
    std::string Name() override;
};

class RootAgreedState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    RootAgreedState() = default;
    __virtual bool GoToRootPort(Machine& machine);
    std::string Name() override;
};

class ReRootState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    ReRootState() = default;
    __virtual bool GoToRootPort(Machine& machine);
    std::string Name() override;
};

class RootForwardState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    RootForwardState() = default;
    __virtual bool GoToRootPort(Machine& machine);
    std::string Name() override;
};

class RootLearnState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    RootLearnState() = default;
    __virtual bool GoToRootPort(Machine& machine);
    std::string Name() override;
};

class ReRootedState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    ReRootedState() = default;
    __virtual bool GoToRootPort(Machine& machine);
    std::string Name() override;
};

class RootPortState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    RootPortState() = default;
    __virtual bool GoToRootProposed(Machine& machine);
    __virtual bool GoToRootAgreed(Machine& machine);
    __virtual bool GoToReRoot(Machine& machine);
    __virtual bool GoToRootForward(Machine& machine);
    __virtual bool GoToRootLearn(Machine& machine);
    __virtual bool GoToReRooted(Machine& machine);
    __virtual bool GoToRootPort(Machine& machine);
    std::string Name() override;
};

// Designated Port States
class DesignatedProposeState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DesignatedProposeState() = default;
    __virtual bool GoToDesignatedPort(Machine& machine);
    std::string Name() override;
};

class DesignatedSyncedState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DesignatedSyncedState() = default;
    __virtual bool GoToDesignatedPort(Machine& machine);
    std::string Name() override;
};

class DesignatedRetiredState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DesignatedRetiredState() = default;
    __virtual bool GoToDesignatedPort(Machine& machine);
    std::string Name() override;
};

class DesignatedForwardState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DesignatedForwardState() = default;
    __virtual bool GoToDesignatedPort(Machine& machine);
    std::string Name() override;
};

class DesignatedLearnState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DesignatedLearnState() = default;
    __virtual bool GoToDesignatedPort(Machine& machine);
    std::string Name() override;
};

class DesignatedDiscardState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DesignatedDiscardState() = default;
    __virtual bool GoToDesignatedPort(Machine& machine);
    std::string Name() override;
};

class DesignatedPortState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    DesignatedPortState() = default;
    __virtual bool GoToDesignatedPropose(Machine& machine);
    __virtual bool GoToDesignatedSynced(Machine& machine);
    __virtual bool GoToDesignatedRetired(Machine& machine);
    __virtual bool GoToDesignatedForward(Machine& machine);
    __virtual bool GoToDesignatedLearn(Machine& machine);
    __virtual bool GoToDesignatedDiscard(Machine& machine);
    std::string Name() override;
};

// Alternate and Backup Port states
class AlternateProposedState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    AlternateProposedState() = default;
    __virtual bool GoToAlternatePort(Machine& machine);
    std::string Name() override;
};

class AlternateAgreedState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    AlternateAgreedState() = default;
    __virtual bool GoToAlternatePort(Machine& machine);
    std::string Name() override;
};

class BlockPortState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BlockPortState() = default;
    __virtual bool GoToAlternatePort(Machine& machine);
    std::string Name() override;
};

class BackupPortState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    BackupPortState() = default;
    __virtual bool GoToAlternatePort(Machine& machine);
    std::string Name() override;
};

class AlternatePortState : public PrtState {
public:
    static State& Instance();
    void Execute(Machine& machine) override;

protected:
    AlternatePortState() = default;
    __virtual bool GoToAlternateProposed(Machine& machine);
    __virtual bool GoToAlternateAgreed(Machine& machine);
    __virtual bool GoToBackupPort(Machine& machine);
    __virtual bool GoToAlternatePort(Machine& machine);
    std::string Name() override;
};

class PrtMachine : public Machine {
public:
    PrtMachine(BridgeH bridge, PortH port);
    std::string Name() override;
};

inline PrtMachine::PrtMachine(BridgeH bridge, PortH port)
    : Machine{ bridge, port, BeginState::Instance() } {
}

inline std::string PrtMachine::Name() {
    return "PRT";
}

// Disabled Port State's names
inline std::string BeginState::Name() {
    return "BEGIN";
}

inline std::string InitPortState::Name() {
    return "INIT_PORT";
}

inline std::string DisablePortState::Name() {
    return "DISABLE_PORT";
}

inline std::string DisabledPortState::Name() {
    return "DISABLED_PORT";
}

// Root Port State's names
inline std::string RootProposedState::Name() {
    return "ROOT_PROPOSED";
}

inline std::string RootAgreedState::Name() {
    return "ROOT_AGREED";
}

inline std::string ReRootState::Name() {
    return "REROOT";
}

inline std::string RootForwardState::Name() {
    return "ROOT_FORWARD";
}

inline std::string RootLearnState::Name() {
    return "ROOT_LEARN";
}

inline std::string ReRootedState::Name() {
    return "REROOT";
}

inline std::string RootPortState::Name() {
    return "ROOT_PORT";
}

// Designated Port State's names
inline std::string DesignatedProposeState::Name() {
    return "DESIGNATED_PROPOSE";
}

inline std::string DesignatedSyncedState::Name() {
    return "DESIGNATED_SYNCED";
}

inline std::string DesignatedRetiredState::Name() {
    return "DESIGNATED_RETIRED";
}

inline std::string DesignatedForwardState::Name() {
    return "DESIGNATED_FORWARD";
}

inline std::string DesignatedLearnState::Name() {
    return "DESIGNATED_LEARN";
}

inline std::string DesignatedDiscardState::Name() {
    return "DESIGNATED_DISCARD";
}

inline std::string DesignatedPortState::Name() {
    return "DESIGNATED_PORT";
}

// Alternate and Backup Port State's names
inline std::string AlternateProposedState::Name() {
    return "ALTERNATE_PROPOSED";
}

inline std::string AlternateAgreedState::Name() {
    return "ALTERNATE_AGREED";
}

inline std::string BlockPortState::Name() {
    return "BLOCK_PORT";
}

inline std::string BackupPortState::Name() {
    return "BACKUP_PORT";
}

inline std::string AlternatePortState::Name() {
    return "ALTERNATE_PORT";
}

} // namespace PortRoleTransitions
} // namespace Stp
