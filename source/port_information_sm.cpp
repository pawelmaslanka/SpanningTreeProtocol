#include "stp/sm/port_information.hpp"
// Dependencies
#include "stp/sm_conditions.hpp"
#include "stp/sm_procedures.hpp"

namespace Stp {
namespace PortInformation {

void PimState::DisabledAction(MachineH machine) {
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

void PimState::AgedAction(MachineH machine) {
    machine.PortInstance().SetInfoIs(Port::Info::Aged);
    machine.PortInstance().SetReselect(true);
    machine.PortInstance().SetSelected(false);
}

void PimState::UpdateAction(MachineH machine) {
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

void PimState::SuperiorDesignatedAction(MachineH machine) {
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

void PimState::RepeatedDesignatedAction(MachineH machine) {
    SmProcedures::RecordProposal(machine.PortInstance());
    SmProcedures::SetTcFlags(machine.PortInstance());
    SmProcedures::UpdtRcvdInfoWhile(machine.PortInstance());
    machine.PortInstance().SetRcvdMsg(false);
}

void PimState::InferiorDesignatedAction(MachineH machine) {
    SmProcedures::RecordDispute(machine.PortInstance());
    machine.PortInstance().SetRcvdMsg(false);
}

void PimState::NotDesignatedAction(MachineH machine) {
    SmProcedures::RecordAgreement(machine.BridgeInstance(), machine.PortInstance());
    SmProcedures::SetTcFlags(machine.PortInstance());
    machine.PortInstance().SetRcvdMsg(false);
}

void PimState::OtherAction(MachineH machine) {
    machine.PortInstance().SetRcvdMsg(false);
}

void PimState::CurrentAction(MachineH machine) {
    std::ignore = machine;
}

void PimState::ReceiveAction(MachineH machine) {
    machine.PortInstance().SetRcvdInfo(SmConditions::RcvInfo(machine.PortInstance()));
}

bool PimState::GoToCurrent(MachineH machine) {
    std::ignore = machine;
    return true; // UCT
}

void PimState::CurrentUctExecute(MachineH machine) {
    if (GoToCurrent(machine)) {
        CurrentAction(machine);
        ChangeState(machine, CurrentState::Instance());
    }
}

StateH BeginState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(BeginState);
}

void BeginState::Execute(MachineH machine) {
    if (GoToDisabled(machine)) {
        DisabledAction(machine);
        ChangeState(machine, DisabledState::Instance());
    }
}

bool BeginState::GoToDisabled(MachineH machine) {
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

StateH DisabledState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(DisabledState);
}

void DisabledState::Execute(MachineH machine) {
    if (GoToAged(machine)) {
        AgedAction(machine);
        ChangeState(machine, AgedState::Instance());
    }
}

bool DisabledState::GoToDisabled(MachineH machine) {
    return machine.PortInstance().RcvdMsg();
}

bool DisabledState::GoToAged(MachineH machine) {
    return machine.PortInstance().PortEnabled();
}

StateH AgedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(AgedState);
}

void AgedState::Execute(MachineH machine) {
    if (GoToUpdate(machine)) {
        UpdateAction(machine);
        ChangeState(machine, UpdateState::Instance());
    }
}

bool AgedState::GoToUpdate(MachineH machine) {
    if (not machine.PortInstance().Selected()) {
        return false;
    }
    else if (not machine.PortInstance().UpdtInfo()) {
        return false;
    }

    return true;
}

StateH UpdateState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(UpdateState);
}

void UpdateState::Execute(MachineH machine) {
    CurrentUctExecute(machine);
}

StateH SuperiorDesignatedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(SuperiorDesignatedState);
}

void SuperiorDesignatedState::Execute(MachineH machine) {
    CurrentUctExecute(machine);
}

StateH RepeatedDesignatedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(RepeatedDesignatedState);
}

void RepeatedDesignatedState::Execute(MachineH machine) {
    CurrentUctExecute(machine);
}

StateH InferiorDesignatedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(InferiorDesignatedState);
}

void InferiorDesignatedState::Execute(MachineH machine) {
    CurrentUctExecute(machine);
}

StateH NotDesignatedState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(NotDesignatedState);
}

void NotDesignatedState::Execute(MachineH machine) {
    CurrentUctExecute(machine);
}

StateH OtherState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(OtherState);
}

void OtherState::Execute(MachineH machine) {
    CurrentUctExecute(machine);
}

StateH CurrentState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(CurrentState);
}

void CurrentState::Execute(MachineH machine) {
    if (GoToReceive(machine)) {
        ReceiveAction(machine);
        ChangeState(machine, ReceiveState::Instance());
    }
}

bool CurrentState::GoToUpdate(MachineH machine) {
    if (not machine.PortInstance().Selected()) {
        return false;
    }
    else if (not machine.PortInstance().UpdtInfo()) {
        return false;
    }

    return true;
}

bool CurrentState::GoToAged(MachineH machine) {
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

bool CurrentState::GoToReceive(MachineH machine) {
    if (not machine.PortInstance().RcvdMsg()) {
        return false;
    }
    else if (machine.PortInstance().UpdtInfo()) {
        return false;
    }

    return true;
}

StateH ReceiveState::Instance() {
    RETURN_STATE_SINGLETON_INSTANCE(ReceiveState);
}

void ReceiveState::Execute(MachineH machine) {
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

bool ReceiveState::GoToSuperiorDesignated(MachineH machine) {
    return Port::RcvdInfo::SuperiorDesignatedInfo == machine.PortInstance().RcvdInfo();
}

bool ReceiveState::GoToRepeatedDesignated(MachineH machine) {
    return Port::RcvdInfo::RepeatedDesignatedInfo == machine.PortInstance().RcvdInfo();
}

bool ReceiveState::GoToInferiorDesignated(MachineH machine) {
    return Port::RcvdInfo::InferiorDesignatedInfo == machine.PortInstance().RcvdInfo();
}

bool ReceiveState::GoToNotDesignated(MachineH machine) {
    return Port::RcvdInfo::InferiorRootAlternateInfo == machine.PortInstance().RcvdInfo();
}

bool ReceiveState::GoToOther(MachineH machine) {
    return Port::RcvdInfo::OtherInfo == machine.PortInstance().RcvdInfo();
}

} // namespace PortInformation
} // namespace Stp
