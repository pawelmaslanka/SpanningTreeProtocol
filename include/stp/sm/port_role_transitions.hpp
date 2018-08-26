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

#define PRT_PREFIX_NAME "PRT @ "
// Disabled Port State's names
constexpr auto kBeginStateName = PRT_PREFIX_NAME "BEGIN";
constexpr auto kInitPortStateName = PRT_PREFIX_NAME "INIT_PORT";
constexpr auto kDisablePortStateName = PRT_PREFIX_NAME "DISABLE_PORT";
constexpr auto kDisabledPortStateName = PRT_PREFIX_NAME "DISABLED_PORT";
// Root Port State's names
constexpr auto kRootProposedStateName = PRT_PREFIX_NAME "ROOT_PROPOSED";
constexpr auto kRootAgreedStateName = PRT_PREFIX_NAME "ROOT_AGREED";
constexpr auto kReRootStateName = PRT_PREFIX_NAME "REROOT";
constexpr auto kRootForwardStateName = PRT_PREFIX_NAME "ROOT_FORWARD";
constexpr auto kRootLearnStateName = PRT_PREFIX_NAME "ROOT_LEARN";
constexpr auto kRootPortStateName = PRT_PREFIX_NAME "ROOT_PORT";
// Designated Port State's names
constexpr auto kDesignatedProposeStateName = PRT_PREFIX_NAME "DESIGNATED_PROPOSE";
constexpr auto kDesignatedSyncedStateName = PRT_PREFIX_NAME "DESIGNATED_SYNCED";
constexpr auto kDesignatedRetiredStateName = PRT_PREFIX_NAME "DESIGNATED_RETIRED";
constexpr auto kDesignatedForwardStateName = PRT_PREFIX_NAME "DESIGNATED_FORWARD";
constexpr auto kDesignatedLearnStateName = PRT_PREFIX_NAME "DESIGNATED_LEARN";
constexpr auto kDesignatedDiscardStateName = PRT_PREFIX_NAME "DESIGNATED_DISCARD";
constexpr auto kDesignatedPortStateName = PRT_PREFIX_NAME "DESIGNATED_PORT";

// Alternate and Backup Port State's names
constexpr auto kAlternateProposedStateName = PRT_PREFIX_NAME "ALTERNATE_PROPOSED";
constexpr auto kAlternateAgreedStateName = PRT_PREFIX_NAME "ALTERNATE_AGREED";
constexpr auto kBlockPortStateName = PRT_PREFIX_NAME "BLOCK_PORT";
constexpr auto kBackupPortStateName = PRT_PREFIX_NAME "BACKUP_PORT";
constexpr auto kAlternatePortStateName = PRT_PREFIX_NAME "ALTERNATE_PORT";

// Disabled Port State's names
inline std::string BeginState::Name() {
    return kBeginStateName;
}

inline std::string InitPortState::Name() {
    return kInitPortStateName;
}

inline std::string DisablePortState::Name() {
    return kDisablePortStateName;
}

inline std::string DisabledPortState::Name() {
    return kDisabledPortStateName;
}

// Root Port State's names
inline std::string RootProposedState::Name() {
    return kRootProposedStateName;
}

inline std::string RootAgreedState::Name() {
    return kRootAgreedStateName;
}

inline std::string ReRootState::Name() {
    return kReRootStateName;
}

inline std::string RootForwardState::Name() {
    return kRootForwardStateName;
}

inline std::string RootLearnState::Name() {
    return kRootLearnStateName;
}

inline std::string ReRootedState::Name() {
    return kReRootStateName;
}

inline std::string RootPortState::Name() {
    return kRootPortStateName;
}

// Designated Port State's names
inline std::string DesignatedProposeState::Name() {
    return kDesignatedProposeStateName;
}

inline std::string DesignatedSyncedState::Name() {
    return kDesignatedSyncedStateName;
}

inline std::string DesignatedRetiredState::Name() {
    return kDesignatedRetiredStateName;
}

inline std::string DesignatedForwardState::Name() {
    return kDesignatedForwardStateName;
}

inline std::string DesignatedLearnState::Name() {
    return kDesignatedLearnStateName;
}

inline std::string DesignatedDiscardState::Name() {
    return kDesignatedDiscardStateName;
}

inline std::string DesignatedPortState::Name() {
    return kDesignatedPortStateName;
}

// Alternate and Backup Port State's names
inline std::string AlternateProposedState::Name() {
    return kAlternateProposedStateName;
}

inline std::string AlternateAgreedState::Name() {
    return kAlternateAgreedStateName;
}

inline std::string BlockPortState::Name() {
    return kBlockPortStateName;
}

inline std::string BackupPortState::Name() {
    return kBackupPortStateName;
}

inline std::string AlternatePortState::Name() {
    return kAlternatePortStateName;
}

} // namespace PortRoleTransitions
} // namespace Stp
