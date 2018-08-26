// This project's headers
#include "stp/sm/port_role_transitions.hpp"
// Dependencies
#include "stp/perf_params.hpp"
#include "stp/sm_conditions.hpp"
#include "stp/sm_parameters.hpp"
#include "stp/sm_procedures.hpp"
#include "stp/time.hpp"

namespace Stp {
namespace PortRoleTransitions {

// Disabled Port States
void PrtState::InitPortAction(Machine& machine) {
    machine.PortInstance().SetRole(PortRole::Disabled);
    machine.PortInstance().SetForward(false);
    machine.PortInstance().SetLearn(false);
    machine.PortInstance().SetSynced(false);
    machine.PortInstance().SetSync(true);
    machine.PortInstance().SetReRoot(true);
    machine.PortInstance().SmTimersInstance().SetRrWhile(SmParams::FwdDelay(machine.PortInstance()));
    machine.PortInstance().SmTimersInstance().SetFdWhile(SmParams::MaxAge(machine.PortInstance()));
    machine.PortInstance().SmTimersInstance().SetRbWhile(0);
}

void PrtState::DisablePortAction(Machine& machine) {
    machine.PortInstance().SetRole(machine.PortInstance().SelectedRole());
    machine.PortInstance().SetForward(false);
    machine.PortInstance().SetLearn(false);
}

void PrtState::DisabledPortAction(Machine& machine) {
    machine.PortInstance().SmTimersInstance().SetFdWhile(SmParams::MaxAge(machine.PortInstance()));
    machine.PortInstance().SetSynced(true);
    machine.PortInstance().SmTimersInstance().SetRrWhile(0);
    machine.PortInstance().SetSync(false);
    machine.PortInstance().SetReRoot(false);
}

// Root Port States
void PrtState::RootProposedAction(Machine& machine) {
    SmProcedures::SetSyncTree(machine.BridgeInstance());
    machine.PortInstance().SetProposed(false);
}

void PrtState::RootAgreedAction(Machine& machine) {
    machine.PortInstance().SetProposed(false);
    machine.PortInstance().SetSync(false);
    machine.PortInstance().SetAgree(true);
    machine.PortInstance().SetNewInfo(true);
}

void PrtState::ReRootAction(Machine& machine) {
    SmProcedures::SetReRootTree(machine.BridgeInstance());
}

void PrtState::RootForwardAction(Machine& machine) {
    machine.PortInstance().SmTimersInstance().SetFdWhile(0);
    machine.PortInstance().SetForward(true);
}

void PrtState::RootLearnAction(Machine& machine) {
    machine.PortInstance().SmTimersInstance().SetFdWhile(
                SmConditions::ForwardDelay(machine.PortInstance()));
    machine.PortInstance().SetLearn(true);
}

void PrtState::ReRootedAction(Machine& machine) {
    machine.PortInstance().SetReRoot(false);
}

void PrtState::RootPortAction(Machine& machine) {
    machine.PortInstance().SetRole(PortRole::Root);
    machine.PortInstance().SmTimersInstance().SetRrWhile(SmParams::FwdDelay(machine.PortInstance()));
}

// Designated Port States
void PrtState::DesignatedProposeAction(Machine& machine) {
    machine.PortInstance().SetProposing(true);
    machine.PortInstance().SmTimersInstance().SetEdgeDelayWhile(
                SmConditions::EdgeDelay(machine.PortInstance()));
    machine.PortInstance().SetNewInfo(true);
}

void PrtState::DesignatedSyncedAction(Machine& machine) {
    machine.PortInstance().SmTimersInstance().SetRrWhile(0);
    machine.PortInstance().SetSynced(true);
    machine.PortInstance().SetSync(false);
}

void PrtState::DesignatedRetiredAction(Machine& machine) {
    machine.PortInstance().SetReRoot(false);
}

void PrtState::DesignatedForwardAction(Machine& machine) {
    machine.PortInstance().SetForward(true);
    machine.PortInstance().SmTimersInstance().SetFdWhile(0);
    machine.PortInstance().SetAgreed(machine.PortInstance().SendRstp());
}

void PrtState::DesignatedLearnAction(Machine& machine) {
    machine.PortInstance().SetLearn(true);
    machine.PortInstance().SmTimersInstance().SetFdWhile(
                SmConditions::ForwardDelay(machine.PortInstance()));
}

void PrtState::DesignatedDiscardAction(Machine& machine) {
    machine.PortInstance().SetDisputed(false);
    machine.PortInstance().SetForward(false);
    machine.PortInstance().SetLearn(false);
    machine.PortInstance().SmTimersInstance().SetFdWhile(
                SmConditions::ForwardDelay(machine.PortInstance()));
}

void PrtState::DesignatedPortAction(Machine& machine) {
    machine.PortInstance().SetRole(PortRole::Designated);
}

// Alternate and Backup States
void PrtState::AlternateProposedAction(Machine& machine) {
    SmProcedures::SetSyncTree(machine.BridgeInstance());
    machine.PortInstance().SetProposed(false);
}

void PrtState::AlternateAgreedAction(Machine& machine) {
    machine.PortInstance().SetProposed(false);
    machine.PortInstance().SetAgree(true);
    machine.PortInstance().SetNewInfo(true);
}

void PrtState::BlockPortAction(Machine& machine) {
    machine.PortInstance().SetRole(machine.PortInstance().SelectedRole());
    machine.PortInstance().SetForward(false);
    machine.PortInstance().SetLearn(false);
}

void PrtState::BackupPortAction(Machine& machine) {
    machine.PortInstance().SmTimersInstance().SetRbWhile(
                2 * PerfParams::HelloTime(machine.PortInstance()));
}

void PrtState::AlternatePortAction(Machine& machine) {
    machine.PortInstance().SmTimersInstance().SetFdWhile(SmParams::FwdDelay(machine.PortInstance()));
    machine.PortInstance().SetSynced(true);
    machine.PortInstance().SmTimersInstance().SetRrWhile(0);
    machine.PortInstance().SetSync(false);
    machine.PortInstance().SetReRoot(false);
}

void PrtState::RootPortUctExecute(Machine& machine) {
    RootPortAction(machine);
    ChangeState(machine, RootPortState::Instance());
}

void PrtState::DesignatedPortUctExecute(Machine& machine) {
    DesignatedPortAction(machine);
    ChangeState(machine, DesignatedPortState::Instance());
}

void PrtState::AlternatePortUctExecute(Machine& machine) {
    AlternatePortAction(machine);
    ChangeState(machine, AlternatePortState::Instance());
}

bool PrtState::ContinueExecute(Machine& machine) {
    if (machine.PortInstance().Role() == machine.PortInstance().SelectedRole()) {
        return true;
    }

    switch (machine.PortInstance().SelectedRole()) {
    case PortRole::Disabled: // Disabled Port States
        DisablePortAction(machine);
        ChangeState(machine, DisablePortState::Instance());
        return false;
    case PortRole::Root: // Root Port States
        RootPortAction(machine);
        ChangeState(machine, RootPortState::Instance());
        return false;
    case PortRole::Designated: // Designated Port States
        DesignatedPortAction(machine);
        ChangeState(machine, DesignatedPortState::Instance());
        return false;
    case PortRole::Alternate: // Alternate and Backup Port States
    case PortRole::Backup:
        BlockPortAction(machine);
        ChangeState(machine, BlockPortState::Instance());
        return false;
    default:
        throw std::runtime_error("Reached invalid port role");
    }

    return true;
}

bool PrtState::TransitionQualified(Machine& machine) {
    if (not machine.PortInstance().Selected()) {
        return false;
    }
    else if (machine.PortInstance().UpdtInfo()) {
        /// @todo Is UpdtInfo make sense here? It appears at
        /// Figure 17-12—RSTP state machines—overview and interrelationships
        /// but it isn't included at state machine diagram
        return false;
    }

    return true;
}

State& BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(Machine& machine) {
    if (GoToInitPort(machine)) {
        InitPortAction(machine);
        ChangeState(machine, InitPortState::Instance());
    }
}

bool BeginState::GoToInitPort(Machine& machine) {
    return machine.BridgeInstance().Begin();
}

State& InitPortState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(InitPortState);
}

void InitPortState::Execute(Machine& machine) {
    if (GoToDisablePort(machine)) {
        DisablePortAction(machine);
        ChangeState(machine, DisablePortState::Instance());
    }
}

bool InitPortState::GoToDisablePort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& DisablePortState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DisablePortState);
}

void DisablePortState::Execute(Machine& machine) {
    if (not ContinueExecute(machine)) {
        return;
    }

    if (GoToDisabledPort(machine)) {
        DisabledPortAction(machine);
        ChangeState(machine, DisabledPortState::Instance());
    }
}

bool DisablePortState::GoToDisabledPort(Machine& machine) {
    if (not TransitionQualified(machine)) {
        return false;
    }
    else if (machine.PortInstance().Learning()) {
        return false;
    }
    else if (machine.PortInstance().Forwarding()) {
        return false;
    }

    return true;;
}

State& DisabledPortState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DisabledPortState);
}

void DisabledPortState::Execute(Machine& machine) {
    if (not ContinueExecute(machine)) {
        return;
    }

    if (GoToDisabledPort(machine)) {
        DisabledPortAction(machine);
        // Leave it as a current state
    }
}

bool DisabledPortState::GoToDisabledPort(Machine& machine) {
    if (not TransitionQualified(machine)) {
        return false;
    }

    if (machine.PortInstance().SmTimersInstance().FdWhile()
            != SmProcedures::MaxAge(machine.PortInstance())) {
        return true;
    }
    else if (machine.PortInstance().Sync()) {
        return true;
    }
    else if (machine.PortInstance().ReRoot()) {
        return true;
    }
    else if (not machine.PortInstance().Synced()) {
        return true;
    }

    return false;
}

// Root Port States
State& RootProposedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(RootProposedState);
}

void RootProposedState::Execute(Machine& machine) {
    RootPortUctExecute(machine);
}

bool RootProposedState::GoToRootPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& RootAgreedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(RootAgreedState);
}

void RootAgreedState::Execute(Machine& machine) {
    RootPortUctExecute(machine);
}

bool RootAgreedState::GoToRootPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& ReRootState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(ReRootState);
}

void ReRootState::Execute(Machine& machine) {
    RootPortUctExecute(machine);
}

bool ReRootState::GoToRootPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& RootForwardState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(RootForwardState);
}

void RootForwardState::Execute(Machine& machine) {
    RootPortUctExecute(machine);
}

bool RootForwardState::GoToRootPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& RootLearnState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(RootLearnState);
}

void RootLearnState::Execute(Machine& machine) {
    RootPortUctExecute(machine);
}

bool RootLearnState::GoToRootPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& ReRootedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(ReRootedState);
}

void ReRootedState::Execute(Machine& machine) {
    RootPortUctExecute(machine);
}

bool ReRootedState::GoToRootPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& RootPortState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(RootPortState);
}

void RootPortState::Execute(Machine& machine) {
    if (not ContinueExecute(machine)) {
        return;
    }

    if (GoToRootProposed(machine)) {
        RootProposedAction(machine);
        ChangeState(machine, RootProposedState::Instance());
    }
    else if (GoToRootAgreed(machine)) {
        RootAgreedAction(machine);
        ChangeState(machine, RootAgreedState::Instance());
    }
    else if (GoToReRoot(machine)) {
        ReRootAction(machine);
        ChangeState(machine, ReRootState::Instance());
    }
    else if (GoToRootForward(machine)) {
        RootForwardAction(machine);
        ChangeState(machine, RootForwardState::Instance());
    }
    else if (GoToRootLearn(machine)) {
        RootLearnAction(machine);
        ChangeState(machine, RootLearnState::Instance());
    }
    else if (GoToReRooted(machine)) {
        ReRootedAction(machine);
        ChangeState(machine, ReRootedState::Instance());
    }
    else if (GoToRootPort(machine)) {
        ReRootedAction(machine);
        // Leave it as a current state
    }
}

bool RootPortState::GoToRootProposed(Machine& machine) {
    if (not machine.PortInstance().Proposed()) {
        return false;
    }
    else if (machine.PortInstance().Agree()) {
        return false;
    }

    return true;
}

bool RootPortState::GoToRootAgreed(Machine& machine) {
    if (SmProcedures::AllSynced(machine.BridgeInstance())) {
        if (not machine.PortInstance().Agree()) {
            return true;
        }
    }

    if (machine.PortInstance().Proposed()) {
        if (machine.PortInstance().Agree()) {
            return true;
        }
    }

    return false;
}

bool RootPortState::GoToReRoot(Machine& machine) {
    if (machine.PortInstance().Forward()) {
        return false;
    }
    else if (machine.PortInstance().ReRoot()) {
        return false;
    }

    return true;
}

bool RootPortState::GoToRootForward(Machine& machine) {
    if (not SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().FdWhile())) {
        if (not SmConditions::ReRooted(machine.BridgeInstance(), machine.PortInstance())) {
            return false;
        }
        else if (not SmTimers::TimedOut(machine.PortInstance().GetSmTimersInstance().RbWhile())) {
            return false;
        }
        else if (not SmConditions::RstpVersion(machine.BridgeInstance())) {
            return false;
        }
    }
    else if (not machine.PortInstance().Learn()) {
        return false;
    }
    else if (machine.PortInstance().Forward()) {
        return false;
    }

    return true;
}

bool RootPortState::GoToRootLearn(Machine& machine) {
    if (not SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().FdWhile())) {
        if (not SmConditions::ReRooted(machine.BridgeInstance(), machine.PortInstance())) {
            return false;
        }
        else if (not SmTimers::TimedOut(machine.PortInstance().GetSmTimersInstance().RbWhile())) {
            return false;
        }
        else if (not SmConditions::RstpVersion(machine.BridgeInstance())) {
            return false;
        }
    }
    else if (machine.PortInstance().Learn()) {
        return false;
    }

    return true;
}

bool RootPortState::GoToReRooted(Machine& machine) {
    if (not machine.PortInstance().ReRoot()) {
        return false;
    }
    else if (not machine.PortInstance().Forward()) {
        return false;
    }

    return true;
}

bool RootPortState::GoToRootPort(Machine& machine) {
    return machine.PortInstance().GetSmTimersInstance().RrWhile()
            != SmParams::FwdDelay(machine.PortInstance());
}

// Designated Port States
State& DesignatedProposeState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DesignatedProposeState);
}

void DesignatedProposeState::Execute(Machine& machine) {
    DesignatedPortUctExecute(machine);
}

bool DesignatedProposeState::GoToDesignatedPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& DesignatedSyncedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DesignatedSyncedState);
}

void DesignatedSyncedState::Execute(Machine& machine) {
    DesignatedPortUctExecute(machine);
}

bool DesignatedSyncedState::GoToDesignatedPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& DesignatedRetiredState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DesignatedRetiredState);
}

void DesignatedRetiredState::Execute(Machine& machine) {
    DesignatedPortUctExecute(machine);
}

bool DesignatedRetiredState::GoToDesignatedPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& DesignatedForwardState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DesignatedForwardState);
}

void DesignatedForwardState::Execute(Machine& machine) {
    DesignatedPortUctExecute(machine);
}

bool DesignatedForwardState::GoToDesignatedPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& DesignatedLearnState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DesignatedLearnState);
}

void DesignatedLearnState::Execute(Machine& machine) {
    DesignatedPortUctExecute(machine);
}

bool DesignatedLearnState::GoToDesignatedPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& DesignatedDiscardState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DesignatedDiscardState);
}

void DesignatedDiscardState::Execute(Machine& machine) {
    DesignatedPortUctExecute(machine);
}

bool DesignatedDiscardState::GoToDesignatedPort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& DesignatedPortState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DesignatedPortState);
}

void DesignatedPortState::Execute(Machine& machine) {
    if (not ContinueExecute(machine)) {
        return;
    }

    if (GoToDesignatedPropose(machine)) {
        DesignatedProposeAction(machine);
        ChangeState(machine, DesignatedProposeState::Instance());
    }
    else if (GoToDesignatedSynced(machine)) {
        DesignatedSyncedAction(machine);
        ChangeState(machine, DesignatedSyncedState::Instance());
    }
    else if (GoToDesignatedRetired(machine)) {
        DesignatedRetiredAction(machine);
        ChangeState(machine, DesignatedRetiredState::Instance());
    }
    else if (GoToDesignatedForward(machine)) {
        DesignatedForwardAction(machine);
        ChangeState(machine, DesignatedForwardState::Instance());
    }
    else if (GoToDesignatedLearn(machine)) {
        DesignatedLearnAction(machine);
        ChangeState(machine, DesignatedLearnState::Instance());
    }
    else if (GoToDesignatedDiscard(machine)) {
        DesignatedDiscardAction(machine);
        ChangeState(machine, DesignatedDiscardState::Instance());
    }
}

bool DesignatedPortState::GoToDesignatedPropose(Machine& machine) {
    if (machine.PortInstance().Forward()) {
        return false;
    }
    else if (machine.PortInstance().Agreed()) {
        return false;
    }
    else if (machine.PortInstance().Proposing()) {
        return false;
    }
    else if (machine.PortInstance().OperEdge()) {
        return false;
    }

    return true;
}

bool DesignatedPortState::GoToDesignatedSynced(Machine& machine) {
    if (machine.PortInstance().Agreed()) {
        if (not machine.PortInstance().Synced()) {
            return true;
        }
    }

    if (machine.PortInstance().OperEdge()) {
        if (not machine.PortInstance().Synced()) {
            return true;
        }
    }

    if (machine.PortInstance().Sync()) {
        if (machine.PortInstance().Synced()) {
            return true;
        }
    }

    if (machine.PortInstance().Learning()) {
        return false;
    }
    else if (machine.PortInstance().Forwarding()) {
        return false;
    }
    else if (machine.PortInstance().Synced()) {
        return false;
    }

    return true;
}

bool DesignatedPortState::GoToDesignatedRetired(Machine& machine) {
    if (not SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().RrWhile())) {
        return false;
    }
    else if (not machine.PortInstance().ReRoot()) {
        return false;
    }

    return true;
}

bool DesignatedPortState::GoToDesignatedForward(Machine& machine) {
    if (machine.PortInstance().Sync()) {
        return false;
    }
    else if (not machine.PortInstance().Learn()) {
        return false;
    }
    else if (machine.PortInstance().Forward()) {
        return false;
    }
    else if (not SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().RrWhile())) {
        if (machine.PortInstance().ReRoot()) {
            return false;
        }
    }

    if (SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().FdWhile())) {
        return true;
    }
    else if (machine.PortInstance().Agreed()) {
        return true;
    }
    else if (machine.PortInstance().OperEdge()) {
        return true;
    }

    return false;
}

bool DesignatedPortState::GoToDesignatedLearn(Machine& machine) {
    if (machine.PortInstance().Sync()) {
        return false;
    }
    else if (machine.PortInstance().Learn()) {
        return false;
    }
    else if (not SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().RrWhile())) {
        if (machine.PortInstance().ReRoot()) {
            return false;
        }
    }

    if (SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().FdWhile())) {
        return true;
    }
    else if (machine.PortInstance().Agreed()) {
        return true;
    }
    else if (machine.PortInstance().OperEdge()) {
        return true;
    }

    return false;
}

bool DesignatedPortState::GoToDesignatedDiscard(Machine& machine) {
    if (machine.PortInstance().OperEdge()) {
        return false;
    }
    else if (not machine.PortInstance().Learn()) {
        if (not machine.PortInstance().Forward()) {
            return false;
        }
    }

    if (machine.PortInstance().Disputed()) {
        return true;
    }
    else if (machine.PortInstance().Sync()) {
        if (not machine.PortInstance().Synced()) {
            return true;
        }
    }
    else if (machine.PortInstance().ReRoot()) {
        if (not SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().RrWhile())) {
            return true;
        }
    }

    return false;
}

// Alternate and Backup Port States
State& AlternateProposedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(AlternateProposedState);
}

void AlternateProposedState::Execute(Machine& machine) {
    AlternatePortUctExecute(machine);
}

bool AlternateProposedState::GoToAlternatePort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& AlternateAgreedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(AlternateAgreedState);
}

void AlternateAgreedState::Execute(Machine& machine) {
    AlternatePortUctExecute(machine);
}

bool AlternateAgreedState::GoToAlternatePort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& BlockPortState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BlockPortState);
}

void BlockPortState::Execute(Machine& machine) {
    if (not ContinueExecute(machine)) {
        return;
    }

    if (GoToAlternatePort(machine)) {
        AlternatePortAction(machine);
        ChangeState(machine, AlternatePortState::Instance());
    }
}

bool BlockPortState::GoToAlternatePort(Machine& machine) {
    if (not TransitionQualified(machine)) {
        return false;
    }
    else if (machine.PortInstance().Learning()) {
        return false;
    }
    else if (machine.PortInstance().Forwarding()) {
        return false;
    }

    return true;
}

State& BackupPortState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BackupPortState);
}

void BackupPortState::Execute(Machine& machine) {
    AlternatePortUctExecute(machine);
}

bool BackupPortState::GoToAlternatePort(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

State& AlternatePortState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(AlternatePortState);
}

void AlternatePortState::Execute(Machine& machine) {
    if (not ContinueExecute(machine)) {
        return;
    }

    if (GoToAlternateProposed(machine)) {
        AlternateProposedAction(machine);
        ChangeState(machine, AlternateProposedState::Instance());
    }
    else if (GoToAlternateAgreed(machine)) {
        AlternateAgreedAction(machine);
        ChangeState(machine, AlternateAgreedState::Instance());
    }
    else if (GoToBackupPort(machine)) {
        AlternateAgreedAction(machine);
        ChangeState(machine, AlternateAgreedState::Instance());
    }
    else if (GoToAlternatePort(machine)) {
        AlternatePortAction(machine);
        // Leave it as a current state
    }
}

bool AlternatePortState::GoToAlternateProposed(Machine& machine) {
    if (not machine.PortInstance().Proposed()) {
        return false;
    }
    else if (machine.PortInstance().Agree()) {
        return false;
    }

    return true;
}

bool AlternatePortState::GoToAlternateAgreed(Machine& machine) {
    if (SmProcedures::AllSynced(machine.BridgeInstance())) {
        if (not machine.PortInstance().Agree()) {
            return true;
        }
    }
    else if (machine.PortInstance().Proposed()) {
        if (machine.PortInstance().Agree()) {
            return true;
        }
    }

    return false;
}

bool AlternatePortState::GoToBackupPort(Machine& machine) {
    if (machine.PortInstance().SmTimersInstance().RbWhile()
            == (2 * PerfParams::HelloTime(machine.PortInstance()))) {
        return false;
    }
    else if (machine.PortInstance().Role() != PortRole::Backup) {
        return false;
    }

    return true;
}

bool AlternatePortState::GoToAlternatePort(Machine& machine) {
    if (machine.PortInstance().SmTimersInstance().FdWhile()
            != SmConditions::ForwardDelay(machine.PortInstance())) {
        return true;
    }
    else if (machine.PortInstance().Sync()) {
        return true;
    }
    else if (machine.PortInstance().ReRoot()) {
        return true;
    }
    else if (not machine.PortInstance().Synced()) {
        return true;
    }

    return false;
}

} // namespace PortRoleTransitions
} // namespace Stp
