#include "stp/sm/port_information.hpp"
// Dependencies
#include "stp/sm_conditions.hpp"
#include "stp/sm_procedures.hpp"

namespace Stp {
namespace PortInformation {

void PimState::DisabledAction(Machine& machine) {
    machine.PortInstance().SetRcvdMsg(false);
    machine.PortInstance().SetProposing(false);
    machine.PortInstance().SetProposed(false);
    machine.PortInstance().SetAgree(false);
    machine.PortInstance().SetAgreed(false);
    machine.PortInstance().SmTimersInstance().SetRcvdInfoWhile(0);
    machine.PortInstance().SetInfoIs(Port::Info::Disabled);
    machine.PortInstance().SetReselect(true);
    machine.PortInstance().SetSelected(false);
}

void PimState::AgedAction(Machine& machine) {
    machine.PortInstance().SetInfoIs(Port::Info::Aged);
    machine.PortInstance().SetReselect(true);
    machine.PortInstance().SetSelected(false);
}

void PimState::UpdateAction(Machine& machine) {
    machine.PortInstance().SetProposing(false);
    machine.PortInstance().SetProposed(false);
    bool agreed = machine.PortInstance().Agreed();
    /// @note Port::Info::Mine is not specified in Standard 802.1D-2004
    agreed = agreed && SmProcedures::BetterOrSameInfo(machine.PortInstance(), Port::Info::Mine);
    machine.PortInstance().SetAgreed(agreed);
    bool synced = machine.PortInstance().Synced();
    synced = synced && agreed;
    machine.PortInstance().SetSynced(synced);
    machine.PortInstance().SetPortPriority(machine.PortInstance().DesignatedPriority());
    machine.PortInstance().SetPortTimes(machine.PortInstance().DesignatedTimes());
    machine.PortInstance().SetUpdtInfo(false);
    machine.PortInstance().SetInfoIs(Port::Info::Mine);
    machine.PortInstance().SetNewInfo(true);
}

void PimState::SuperiorDesignatedAction(Machine& machine) {
    machine.PortInstance().SetAgreed(false);
    machine.PortInstance().SetProposing(false);
    SmProcedures::RecordProposal(machine.PortInstance());
    SmProcedures::SetTcFlags(machine.PortInstance());
    bool agree = machine.PortInstance().Agree();
    /// @note Port::Info::Received is not specified in Standard 802.1D-2004
    agree = agree && SmProcedures::BetterOrSameInfo(machine.PortInstance(), Port::Info::Received);
    machine.PortInstance().SetAgree(agree);
    SmProcedures::RecordPriority(machine.PortInstance());
    SmProcedures::RecordTimes(machine.PortInstance());
    SmProcedures::UpdtRcvdInfoWhile(machine.PortInstance());
    machine.PortInstance().SetInfoIs(Port::Info::Received);
    machine.PortInstance().SetReselect(true);
    machine.PortInstance().SetSelected(false);
    machine.PortInstance().SetRcvdMsg(false);
}

void PimState::RepeatedDesignatedAction(Machine& machine) {
    SmProcedures::RecordProposal(machine.PortInstance());
    SmProcedures::SetTcFlags(machine.PortInstance());
    SmProcedures::UpdtRcvdInfoWhile(machine.PortInstance());
    machine.PortInstance().SetRcvdMsg(false);
}

void PimState::InferiorDesignatedAction(Machine& machine) {
    SmProcedures::RecordDispute(machine.PortInstance());
    machine.PortInstance().SetRcvdMsg(false);
}

void PimState::NotDesignatedAction(Machine& machine) {
    SmProcedures::RecordAgreement(machine.BridgeInstance(), machine.PortInstance());
    SmProcedures::SetTcFlags(machine.PortInstance());
    machine.PortInstance().SetRcvdMsg(false);
}

void PimState::OtherAction(Machine& machine) {
    machine.PortInstance().SetRcvdMsg(false);
}

void PimState::CurrentAction(Machine& machine) {
    std::ignore = machine;
}

void PimState::ReceiveAction(Machine& machine) {
    machine.PortInstance().SetRcvdInfo(SmConditions::RcvInfo(machine.PortInstance()));
}

bool PimState::GoToCurrent(Machine& machine) {
    std::ignore = machine;
    return true; // UCT
}

void PimState::CurrentUctExecute(Machine& machine) {
    if (GoToCurrent(machine)) {
        CurrentAction(machine);
        ChangeState(machine, CurrentState::Instance());
    }
}

State& BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(Machine& machine) {
    if (GoToDisabled(machine)) {
        DisabledAction(machine);
        ChangeState(machine, DisabledState::Instance());
    }
}

bool BeginState::GoToDisabled(Machine& machine) {
    if (machine.BridgeInstance().Begin()) {
        return true;
    }

    if (machine.PortInstance().PortEnabled()) {
        return false;
    }
    else if (Port::Info::Disabled == machine.PortInstance().InfoIs()) {
        return false;
    }

    return true;
}

State& DisabledState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DisabledState);
}

void DisabledState::Execute(Machine& machine) {
    if (GoToAged(machine)) {
        AgedAction(machine);
        ChangeState(machine, AgedState::Instance());
    }
}

bool DisabledState::GoToDisabled(Machine& machine) {
    return machine.PortInstance().RcvdMsg();
}

bool DisabledState::GoToAged(Machine& machine) {
    return machine.PortInstance().PortEnabled();
}

State& AgedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(AgedState);
}

void AgedState::Execute(Machine& machine) {
    if (GoToUpdate(machine)) {
        UpdateAction(machine);
        ChangeState(machine, UpdateState::Instance());
    }
}

bool AgedState::GoToUpdate(Machine& machine) {
    if (not machine.PortInstance().Selected()) {
        return false;
    }
    else if (not machine.PortInstance().UpdtInfo()) {
        return false;
    }

    return true;
}

State& UpdateState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(UpdateState);
}

void UpdateState::Execute(Machine& machine) {
    CurrentUctExecute(machine);
}

State& SuperiorDesignatedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(SuperiorDesignatedState);
}

void SuperiorDesignatedState::Execute(Machine& machine) {
    CurrentUctExecute(machine);
}

State& RepeatedDesignatedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(RepeatedDesignatedState);
}

void RepeatedDesignatedState::Execute(Machine& machine) {
    CurrentUctExecute(machine);
}

State& InferiorDesignatedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(InferiorDesignatedState);
}

void InferiorDesignatedState::Execute(Machine& machine) {
    CurrentUctExecute(machine);
}

State& NotDesignatedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(NotDesignatedState);
}

void NotDesignatedState::Execute(Machine& machine) {
    CurrentUctExecute(machine);
}

State& OtherState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(OtherState);
}

void OtherState::Execute(Machine& machine) {
    CurrentUctExecute(machine);
}

State& CurrentState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(CurrentState);
}

void CurrentState::Execute(Machine& machine) {
    if (GoToReceive(machine)) {
        ReceiveAction(machine);
        ChangeState(machine, ReceiveState::Instance());
    }
}

bool CurrentState::GoToUpdate(Machine& machine) {
    if (not machine.PortInstance().Selected()) {
        return false;
    }
    else if (not machine.PortInstance().UpdtInfo()) {
        return false;
    }

    return true;
}

bool CurrentState::GoToAged(Machine& machine) {
    if (Port::Info::Received != machine.PortInstance().InfoIs()) {
        return false;
    }
    else if (not SmTimers::TimedOut(machine.PortInstance().SmTimersInstance().RcvdInfoWhile())) {
        return false;
    }
    else if (machine.PortInstance().UpdtInfo()) {
        return false;
    }
    else if (machine.PortInstance().RcvdMsg()) {
        return false;
    }

    return true;
}

bool CurrentState::GoToReceive(Machine& machine) {
    if (not machine.PortInstance().RcvdMsg()) {
        return false;
    }
    else if (machine.PortInstance().UpdtInfo()) {
        return false;
    }

    return true;
}

State& ReceiveState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(ReceiveState);
}

void ReceiveState::Execute(Machine& machine) {
    if (GoToSuperiorDesignated(machine)) {
        SuperiorDesignatedAction(machine);
        ChangeState(machine, SuperiorDesignatedState::Instance());
    }
    else if (GoToRepeatedDesignated(machine)) {
        RepeatedDesignatedAction(machine);
        ChangeState(machine, RepeatedDesignatedState::Instance());
    }
    else if (GoToInferiorDesignated(machine)) {
        InferiorDesignatedAction(machine);
        ChangeState(machine, InferiorDesignatedState::Instance());
    }
    else if (GoToNotDesignated(machine)) {
        NotDesignatedAction(machine);
        ChangeState(machine, NotDesignatedState::Instance());
    }
    else if (GoToOther(machine)) {
        OtherAction(machine);
        ChangeState(machine, OtherState::Instance());
    }
}

bool ReceiveState::GoToSuperiorDesignated(Machine& machine) {
    return Port::RcvdInfo::SuperiorDesignatedInfo == machine.PortInstance().RcvdInfo();
}

bool ReceiveState::GoToRepeatedDesignated(Machine& machine) {
    return Port::RcvdInfo::RepeatedDesignatedInfo == machine.PortInstance().RcvdInfo();
}

bool ReceiveState::GoToInferiorDesignated(Machine& machine) {
    return Port::RcvdInfo::InferiorDesignatedInfo == machine.PortInstance().RcvdInfo();
}

bool ReceiveState::GoToNotDesignated(Machine& machine) {
    return Port::RcvdInfo::InferiorRootAlternateInfo == machine.PortInstance().RcvdInfo();
}

bool ReceiveState::GoToOther(Machine& machine) {
    return Port::RcvdInfo::OtherInfo == machine.PortInstance().RcvdInfo();
}

} // namespace PortInformation
} // namespace Stp
