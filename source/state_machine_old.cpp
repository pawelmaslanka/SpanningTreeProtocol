/***************************************************************************************************
Copyright (c) 2018, Pawel Maslanka <pawmas@hotmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
***************************************************************************************************/

// This project's headers
#include "port_id.hpp"
#include "spanning_tree_protocol.hpp"
#include "time.hpp"

// C++ Standard Library
#include <iostream>

namespace Rstp {

SpanningTreeProtocol::SpanningTreeProtocol(Bridge& bridge, Management& interface) noexcept
    : _bridge{ bridge }, _ports{ bridge.Ports }, _interface{ interface }
{
    // Nothing more to do
}

void SpanningTreeProtocol::TickEvent() noexcept
{
    for (Port& portMap : _ports) {
        portMap.SetTick(true);
        PortTimers(portMap);
        PortReceive(portMap);
        PortProtocolMigration(portMap);
        BridgeDetection(portMap);
        PortTransmit(portMap);
        PortInformation(portMap);
        PortRoleSelection(portMap);
        PortRoleTransitions(portMap);
        PortStateTransition(portMap);
        TopologyChange(portMap);
    }
}

void SpanningTreeProtocol::PortTimers(Port& port) noexcept
{
    switch (port.PtiState()) {
    case PtiState::OneSecond:
        if (port.Tick()) {
            goto PTI_TICK;
        }

        break;

        PTI_ONE_SECOND:
        port.SetPtiState(PtiState::OneSecond);
        PortTimersAction(port);
        break;
    case PtiState::Tick:
        goto PTI_ONE_SECOND; // UCT

        PTI_TICK:
        port.SetPtiState(PtiState::Tick);
        PortTimersAction(port);
        break;
    case PtiState::Begin:
        if (_bridge.Begin()) {
            goto PTI_ONE_SECOND;
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::PortTimersAction(Port& port) noexcept
{
    switch (port.PtiState()) {
    case PtiState::OneSecond:
        port.SetTick(false);
        break;
    case PtiState::Tick:
        --port.GetSmTimers();
        port.DecTxCount();
        port.DecAgeingTime();

        /// @todo Should machine state be responsible for flushing data base entries?
        //        if (0 == port.AgeingTime()) {
        //            _interface.PortFdbFlush(port.portId.PortNum());
        //        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::PortReceive(Port& port) noexcept
{
    switch (port.PrxState()) {
    case PrxState::Discard:
        if (port.RcvdBpdu() && port.PortEnabled()) {
            goto PRX_RECEIVE;
        }

        break;
PRX_DISCARD:
        port.SetPrxState(PrxState::Discard);
        PortReceiveAction(port);
        break;
    case PrxState::Receive:
        if (port.RcvdBpdu() && port.PortEnabled() && not port.RcvdMsg()) {
            goto PRX_RECEIVE;
        }

        break;
PRX_RECEIVE:
        port.SetPrxState(PrxState::Receive);
        PortReceiveAction(port);
        break;
    case PrxState::Begin:
        if (_bridge.Begin() || ((port.RcvdBpdu()  || (port.SmTimers().EdgeDelayWhile() != MigrateTime()))
                                && not port.PortEnabled())) {
            goto PRX_DISCARD;
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::PortReceiveAction(Port& port) noexcept
{
    switch (port.PrxState()) {
    case PrxState::Discard:
        port.SetRcvdBpdu(false);
        port.SetRcvdMsg(false);
        port.SetRcvdRstp(false);
        port.SetRcvdStp(false);
        port.GetSmTimers().SetEdgeDelayWhile(MigrateTime());
        break;
    case PrxState::Receive:
        UpdtBpduVersion(port);
        port.SetOperEdge(false);
        port.SetRcvdBpdu(false);
        port.SetRcvdMsg(true);
        port.GetSmTimers().SetEdgeDelayWhile(MigrateTime());
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::PortProtocolMigration(Port& port) noexcept
{
    switch (port.PpmState()) {
    case PpmState::CheckingRstp:
        if (Timer::TimedOut(port.SmTimers().MdelayWhile())) {
            goto PPM_SENSING;
        }
        else if ((port.SmTimers().MdelayWhile() != MigrateTime())
                 && not port.PortEnabled()) {
            goto PPM_CHECKING_RSTP;
        }
        else {
            break;
        }

        PPM_CHECKING_RSTP:
        port.SetPpmState(PpmState::CheckingRstp);
        PortProtocolMigrationAction(port);
        break;
    case PpmState::SelectingStp:
        if (Timer::TimedOut(port.SmTimers().MdelayWhile())
                || not port.PortEnabled() || port.Mcheck()) {
            goto PPM_SENSING;
        }

        break;
        PPM_SELECTING_STP:
        port.SetPpmState(PpmState::SelectingStp);
        PortProtocolMigrationAction(port);
        break;
    case PpmState::Sensing:
        if (not port.PortEnabled() || port.Mcheck()
                || (RstpVersion() && not port.SendRstp() && port.RcvdRstp())) {
            goto PPM_CHECKING_RSTP;
        }
        else if (port.SendRstp() && port.RcvdStp()) {
            goto PPM_SELECTING_STP;
        }

        break;
        PPM_SENSING:
        port.SetPpmState(PpmState::Sensing);
        PortProtocolMigrationAction(port);
        break;
    case PpmState::Begin:
        if (_bridge.Begin()) {
            goto PPM_CHECKING_RSTP;
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::PortProtocolMigrationAction(Port &port) noexcept
{
    switch (port.PpmState()) {
    case PpmState::CheckingRstp:
        port.SetMcheck(false);
        port.SetSendRstp(RstpVersion());
        port.GetSmTimers().SetMdelayWhile((Time::BaseType)Time::RecommendedValue::MigrateTime);
        break;
    case PpmState::SelectingStp:
        port.SetSendRstp(false);
        port.GetSmTimers().SetMdelayWhile((Time::BaseType)Time::RecommendedValue::MigrateTime);
        break;
    case PpmState::Sensing:
        port.SetRcvdRstp(false);
        port.SetRcvdStp(false);
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::BridgeDetection(Port& port) noexcept
{
    switch (port.BdmState()) {
    case BdmState::Edge:
        if ((not port.PortEnabled() && not port.adminEdge) || not port.OperEdge()) {
            goto BDM_NOT_EDGE;
        }

        break;
        BDM_EDGE:
        port.SetBdmState(BdmState::Edge);
        BridgeDetectionAction(port);
        break;
    case BdmState::NotEdge:
        if ((not port.PortEnabled() && AdminEdge(port))
                || (Timer::TimedOut(port.SmTimers().EdgeDelayWhile()) && AutoEdge(port)
                    && port.SendRstp() && port.Proposing())) {
            goto BDM_EDGE;
        }

        break;
        BDM_NOT_EDGE:
        port.SetBdmState(BdmState::NotEdge);
        BridgeDetectionAction(port);
        break;
    case BdmState::Begin:
        if (not _bridge.Begin()) {
            break;
        }

        if (port.adminEdge) {
            goto BDM_EDGE;
        }
        else {
            goto BDM_NOT_EDGE;
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::BridgeDetectionAction(Port& port) noexcept
{
    switch (port.BdmState()) {
    case BdmState::Edge:
        port.SetOperEdge(true);
        break;
    case BdmState::NotEdge:
        port.SetOperEdge(false);
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::PortTransmit(Port& port) noexcept
{
    switch (port.PtxState()) {
    case PtxState::TransmitInit:
        goto PTX_IDLE; // UCT

        PTX_TRANSMIT_INIT:
        port.SetPtxState(PtxState::TransmitInit);
        PortTransmitAction(port);
        break;
    case PtxState::TransmitPeriodic:
        goto PTX_IDLE; // UCT

        PTX_TRANSMIT_PERIODIC:
        port.SetPtxState(PtxState::TransmitPeriodic);
        PortTransmitAction(port);
        break;
    case PtxState::TransmitConfig:
        goto PTX_IDLE; // UCT

        PTX_TRANSMIT_CONFIG:
        port.SetPtxState(PtxState::TransmitConfig);
        PortTransmitAction(port);
        break;
    case PtxState::TransmitTcn:
        goto PTX_IDLE; // UCT

        PTX_TRANSMIT_TCN:
        port.SetPtxState(PtxState::TransmitTcn);
        PortTransmitAction(port);
        break;
    case PtxState::TransmitRstp:
        goto PTX_IDLE; // UCT

        PTX_TRANSMIT_RSTP:
        port.SetPtxState(PtxState::TransmitRstp);
        PortTransmitAction(port);
        break;
    case PtxState::Idle: {
        if (not (port.Selected() && not port.UpdtInfo())) {
            break;
        }

        if (Timer::TimedOut(port.SmTimers().HelloWhen())) {
            goto PTX_TRANSMIT_PERIODIC;
        }

        if (port.SendRstp() && port.NewInfo() && (port.TxCount() < TxHoldCount())
                /// @note We checked that HelloWhen != 0
                /* && (not Timer::TimedOut(port.SmTimers().HelloWhen())) */) {
            goto PTX_TRANSMIT_RSTP;
        }

        if (not port.SendRstp() && port.NewInfo() && (PortRole::Root == port.Role())
                && (port.TxCount() < TxHoldCount())
                /// @note We checked that HelloWhen != 0
                /* && (not Timer::TimedOut(port.SmTimers().HelloWhen())) */) {
            goto PTX_TRANSMIT_TCN;
        }

        if (not port.SendRstp() && port.NewInfo() && (PortRole::Designated == port.Role())
                && (port.TxCount() < TxHoldCount())
                /// @note We checked that HelloWhen != 0
                /* && (not Timer::TimedOut(port.SmTimers().HelloWhen())) */) {
            goto PTX_TRANSMIT_CONFIG;
        }

        break;

        PTX_IDLE:
        port.SetPtxState(PtxState::Idle);
        PortTransmitAction(port);
        break;
    }
    case PtxState::Begin:
        if (_bridge.Begin()) {
            goto PTX_TRANSMIT_INIT;
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::PortTransmitAction(Port &port) noexcept
{
    switch (port.PtxState()) {
    case PtxState::TransmitInit:
        port.SetNewInfo(true);
        port.SetTxCount(0);
        break;
    case PtxState::TransmitPeriodic:
        port.SetNewInfo(port.NewInfo() || ((PortRole::Designated == port.Role())
                                           || ((PortRole::Root == port.Role())
                                               && (not Timer::TimedOut(port.SmTimers().TcWhile())))));
        break;
    case PtxState::TransmitConfig:
        port.SetNewInfo(false);
        TxConfig(port);
        port.IncTxCount();
        port.SetTcAck(false);
        break;
    case PtxState::TransmitTcn:
        port.SetNewInfo(false);
        TxTcn(port);
        port.IncTxCount();
        break;
    case PtxState::TransmitRstp:
        port.SetNewInfo(false);
        TxRstp(port);
        port.IncTxCount();
        port.SetTcAck(false);
        break;
    case PtxState::Idle:
        port.GetSmTimers().SetHelloWhen(HelloTime(port));
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::PortInformation(Port& port) noexcept
{
    switch (port.PimState()) {
    case PimState::Disabled:
        if (port.RcvdMsg()) {
            goto PIM_DISABLED;
        }

        if (port.PortEnabled()) {
            goto PIM_AGED;
        }

        break;
        PIM_DISABLED:
        port.SetPimState(PimState::Disabled);
        PortInformationAction(port);
        break;
    case PimState::Aged:
        if (port.Selected() && port.UpdtInfo()) {
            goto PIM_UPDATE;
        }

        break;
        PIM_AGED:
        port.SetPimState(PimState::Aged);
        PortInformationAction(port);
        break;
    case PimState::Update:
        goto PIM_CURRENT; // UCT

        PIM_UPDATE:
        port.SetPimState(PimState::Update);
        PortInformationAction(port);
        break;
    case PimState::SuperiorDesignated:
        goto PIM_CURRENT; // UCT

        PIM_SUPERIOR_DESIGNATED:
        port.SetPimState(PimState::SuperiorDesignated);
        PortInformationAction(port);
        break;
    case PimState::RepeatedDesignated:
        goto PIM_CURRENT; // UCT

        PIM_REPEATED_DESIGNATED:
        port.SetPimState(PimState::RepeatedDesignated);
        PortInformationAction(port);
        break;
    case PimState::InferiorDesignated:
        goto PIM_CURRENT; // UCT

        PIM_INFERIOR_DESIGNATED:
        port.SetPimState(PimState::InferiorDesignated);
        PortInformationAction(port);
        break;
    case PimState::NotDesignated:
        goto PIM_CURRENT; // UCT

        PIM_NOT_DESIGNATED:
        port.SetPimState(PimState::NotDesignated);
        PortInformationAction(port);
        break;
    case PimState::Other:
        goto PIM_CURRENT; // UCT

        PIM_OTHER:
        port.SetPimState(PimState::Other);
        PortInformationAction(port);
        break;
    case PimState::Current:
        if (port.Selected() && port.UpdtInfo()) {
            goto PIM_UPDATE;
        }
        else if (Port::Info::Received == port.InfoIs()
                 && Timer::TimedOut(port.SmTimers().RcvdInfoWhile())
                 && not port.UpdtInfo() && not port.RcvdMsg()) {
            goto PIM_AGED;
        }
        else if (port.RcvdMsg() && not port.UpdtInfo()) {
            goto PIM_RECEIVE;
        }

        break;
        PIM_CURRENT:
        port.SetPimState(PimState::Current);
        PortInformationAction(port);
        break;
    case PimState::Receive:
        if (Port::RcvdInfo::SuperiorDesignatedInfo == port.RcvdInfo()) {
            goto PIM_SUPERIOR_DESIGNATED;
        }

        if (Port::RcvdInfo::RepeatedDesignatedInfo == port.RcvdInfo()) {
            goto PIM_REPEATED_DESIGNATED;
        }

        if (Port::RcvdInfo::InferiorDesignatedInfo == port.RcvdInfo()) {
            goto PIM_INFERIOR_DESIGNATED;
        }

        if (Port::RcvdInfo::InferiorRootAlternateInfo == port.RcvdInfo()) {
            goto PIM_NOT_DESIGNATED;
        }

        if (Port::RcvdInfo::OtherInfo == port.RcvdInfo()) {
            goto PIM_OTHER;
        }

        break;
        PIM_RECEIVE:
        port.SetPimState(PimState::Receive);
        PortInformationAction(port);
        break;
    case PimState::Begin:
        if (_bridge.Begin() || (not port.PortEnabled() && (port.InfoIs() != Port::Info::Disabled))) {
            goto PIM_DISABLED;
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::PortInformationAction(Port& port) noexcept
{
    switch (port.PimState()) {
    case PimState::Disabled:
        port.SetAgree(false);
        port.SetAgreed(false);
        port.SetProposed(false);
        port.SetProposing(false);
        port.SetRcvdMsg(false);
        port.GetSmTimers().SetRcvdInfoWhile(0);
        port.SetInfoIs(Port::Info::Disabled);
        port.SetReselect(true);
        port.SetSelected(false);
        break;
    case PimState::Aged:
        port.SetInfoIs(Port::Info::Aged);
        port.SetReselect(true);
        port.SetSelected(false);
        break;
    case PimState::Update:
        port.SetProposed(false);
        port.SetProposing(false);
        /// @todo Check if BetterOrSameInfo() arguments is fine
        port.SetAgreed(port.Agreed() && BetterOrSameInfo(port, Port::Info::Mine));
        port.SetSynced(port.Synced() && port.Agreed());
        port.SetPortPriority(port.DesignatedPriority());
        port.SetPortTimes(port.DesignatedTimes());
        port.SetUpdtInfo(false);
        port.SetInfoIs(Port::Info::Mine);
        port.SetNewInfo(true);
        break;
    case PimState::SuperiorDesignated:
        port.SetAgreed(false);
        port.SetProposing(false);
        RecordProposal(port);
        SetTcFlags(port);
        /// @todo Check if BetterOrSameInfo() arguments is fine
        port.SetAgree(port.Agree() && BetterOrSameInfo(port, Port::Info::Received));
        RecordPriority(port);
        RecordTimes(port);
        UpdtRcvdInfoWhile(port);
        port.SetInfoIs(Port::Info::Received);
        port.SetReselect(true);
        port.SetRcvdMsg(false);
        port.SetSelected(false);
        break;
    case PimState::RepeatedDesignated:
        RecordProposal(port);
        SetTcFlags(port);
        UpdtRcvdInfoWhile(port);
        port.SetRcvdMsg(false);
        break;
    case PimState::InferiorDesignated:
        RecordDispute(port);
        port.SetRcvdMsg(false);
        break;
    case PimState::NotDesignated:
        RecordAgreement(port);
        SetTcFlags(port);
        port.SetRcvdMsg(false);
        break;
    case PimState::Other:
        port.SetRcvdMsg(false);
        break;
    case PimState::Current:
        break;
    case PimState::Receive:
        port.SetRcvdInfo(RcvInfo(port));
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::PortRoleSelection(Port& port) noexcept
{
    switch (port.PrsState()) {
    case PrsState::InitBridge:
        goto PRS_ROLE_SELECTION; // UCT

        PRS_INIT_BRIDGE:
        port.SetPrsState(PrsState::InitBridge);
        PortRoleSelectionAction(port);
        break;
    case PrsState::RoleSelection: {
        for (Port& portMap : _ports) {
            if (portMap.Reselect()) {
                goto PRS_ROLE_SELECTION;
            }
        }

        break;
        PRS_ROLE_SELECTION:
        port.SetPrsState(PrsState::RoleSelection);
        PortRoleSelectionAction(port);
        break;
    }
    case PrsState::Begin:
        if (_bridge.Begin()) {
            goto PRS_INIT_BRIDGE;
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::PortRoleSelectionAction(Port& port) noexcept
{
    switch (port.PrsState()) {
    case PrsState::InitBridge:
        UpdtRoleDisabledTree();
        break;
    case PrsState::RoleSelection:
        ClearReselectTree();
        UpdtRolesTree();
        SetSelectedTree();
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::PortRoleTransitions(Port& port) noexcept
{
    if (PrtState::Begin == port.PrtState()) {
        PortRoleTransitionsDisabledState(port);
        return;
    }

    switch(port.SelectedRole()) {
    case PortRole::Disabled:
        if (port.Role() != port.SelectedRole()) {
            port.SetPrtState(PrtState::DisablePort);
            PortRoleTransitionsDisabledStateAction(port);
            break;
        }

        PortRoleTransitionsDisabledState(port);
        break;
    case PortRole::Root:
        if (port.Role() != port.SelectedRole()) {
            port.SetPrtState(PrtState::RootPort);
            PortRoleTransitionsRootStateAction(port);
            break;
        }

        PortRoleTransitionsRootState(port);
        break;
    case PortRole::Designated:
        if (port.Role() != port.SelectedRole()) {
            port.SetPrtState(PrtState::DesignatedPort);
            PortRoleTransitionsDesignatedStateAction(port);
            break;
        }

        PortRoleTransitionsDesignatedState(port);
        break;
    default:
        //        else if ((PortRole::Alternate == port.selectedRole)
        //                 || (PortRole::Backup == port.selectedRole))
        if (port.Role() != port.SelectedRole()) {
            port.SetPrtState(PrtState::BlockPort);
            PortRoleTransitionsAlternateAndBackupStateAction(port);
            return;
        }

        PortRoleTransitionsAlternateAndBackupState(port);
        break;
    }
}

void SpanningTreeProtocol::PortRoleTransitionsDisabledState(Port& port) noexcept
{
    switch (port.PrtState()) {
    case PrtState::InitPort:
        goto PRT_DISABLE_PORT; // UCT

        PRT_INIT_PORT:
        port.SetPrtState(PrtState::InitPort);
        PortRoleTransitionsDisabledStateAction(port);
        break;
    case PrtState::DisablePort:
        if (not (port.Selected() && not port.UpdtInfo())) {
            break;
        }

        if (not port.Learning() && not port.Forwarding()) {
            goto PRT_DISABLED_PORT;
        }

        break;
        PRT_DISABLE_PORT:
        port.SetPrtState(PrtState::DisablePort);
        PortRoleTransitionsDisabledStateAction(port);
        break;
    case PrtState::DisabledPort:
        if (not (port.Selected() && not port.UpdtInfo())) {
            break;
        }

        if ((port.SmTimers().FdWhile() != MaxAge(port)) || port.Sync() || port.ReRoot() || not port.Synced()) {
            goto PRT_DISABLED_PORT;
        }

        break;
        PRT_DISABLED_PORT:
        port.SetPrtState(PrtState::DisabledPort);
        PortRoleTransitionsDisabledStateAction(port);
        break;
    case PrtState::Begin:
        if (_bridge.Begin()) {
            goto PRT_INIT_PORT;
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::PortRoleTransitionsDisabledStateAction(Port& port) noexcept
{
    switch (port.PrtState()) {
    case PrtState::InitPort:
        port.SetRole(PortRole::Disabled);
        port.SetForward(false);
        port.SetLearn(false);
        port.SetSynced(false);
        port.SetSync(true);
        port.SetReRoot(true);
        port.GetSmTimers().SetRrWhile(FwdDelay(port));
        port.GetSmTimers().SetFdWhile(MaxAge(port));
        port.GetSmTimers().SetRbWhile(0);
        break;
    case PrtState::DisablePort:
        port.SetRole(port.SelectedRole());
        port.SetForward(false);
        port.SetLearn(false);
        break;
    case PrtState::DisabledPort:
        port.GetSmTimers().SetFdWhile(MaxAge(port));
        port.SetSynced(true);
        port.GetSmTimers().SetRrWhile(0);
        port.SetSync(false);
        port.SetReRoot(false);
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::PortRoleTransitionsRootState(Port& port) noexcept
{
    switch (port.PrtState()) {
    case PrtState::RootProposed:
        goto PRT_ROOT_PORT; // UCT

        PRT_ROOT_PROPOSED:
        port.SetPrtState(PrtState::RootProposed);
        PortRoleTransitionsRootStateAction(port);
        break;
    case PrtState::RootAgreed:
        goto PRT_ROOT_PORT; // UCT

        PRT_ROOT_AGREED:
        port.SetPrtState(PrtState::RootAgreed);
        PortRoleTransitionsRootStateAction(port);
        break;
    case PrtState::Reroot:
        goto PRT_ROOT_PORT; // UCT

        PRT_REROOT:
        port.SetPrtState(PrtState::Reroot);
        PortRoleTransitionsRootStateAction(port);
        break;
    case PrtState::RootForward:
        goto PRT_ROOT_PORT; // UCT

        PRT_ROOT_FORWARD:
        port.SetPrtState(PrtState::RootForward);
        PortRoleTransitionsRootStateAction(port);
        break;
    case PrtState::RootLearn:
        goto PRT_ROOT_PORT; // UCT

        PRT_ROOT_LEARN:
        port.SetPrtState(PrtState::RootLearn);
        PortRoleTransitionsRootStateAction(port);
        break;
    case PrtState::Rerooted:
        goto PRT_ROOT_PORT; // UCT

        PRT_REROOTED:
        port.SetPrtState(PrtState::Rerooted);
        PortRoleTransitionsRootStateAction(port);
        break;
    case PrtState::RootPort:
        if (not (port.Selected() && not port.UpdtInfo())) {
            break;
        }

        if (port.Proposed() && not port.Agree()) {
            goto PRT_ROOT_PROPOSED;
        }

        if ((AllSynced() && not port.Agree()) || (port.Proposed() && port.Agree())) {
            goto PRT_ROOT_AGREED;
        }

        if (not port.Forward() && not port.ReRoot()) {
            goto PRT_REROOT;
        }

        if (port.SmTimers().RrWhile() != FwdDelay(port)) {
            goto PRT_ROOT_PORT;
        }

        if (port.ReRoot() && port.Forward()) {
            goto PRT_REROOTED;
        }

        if ((Timer::TimedOut(port.SmTimers().FdWhile())
                  || ((ReRooted(port) && Timer::TimedOut(port.SmTimers().RbWhile()))
                      && RstpVersion())) && not port.Learn()) {
            goto PRT_ROOT_LEARN;
        }

        if (((Timer::TimedOut(port.SmTimers().FdWhile())
                   || ((ReRooted(port) && Timer::TimedOut(port.SmTimers().RbWhile()))
                       && RstpVersion())) && port.Learn() && not port.Forward())) {
            goto PRT_ROOT_FORWARD;
        }

        break;
        PRT_ROOT_PORT:
        port.SetPrtState(PrtState::RootPort);
        PortRoleTransitionsRootStateAction(port);
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::PortRoleTransitionsRootStateAction(Port& port) noexcept
{
    switch (port.PrtState()) {
    case PrtState::RootProposed:
        SetSyncTree();
        port.SetProposed(false);
        break;
    case PrtState::RootAgreed:
        port.SetProposed(false);
        port.SetSync(false);
        port.SetAgree(true);
        port.SetNewInfo(true);
        break;
    case PrtState::Reroot:
        SetReRootTree();
        break;
    case PrtState::RootForward:
        port.GetSmTimers().SetFdWhile(0);
        port.SetForward(true);
        break;
    case PrtState::RootLearn:
        port.GetSmTimers().SetFdWhile(ForwardDelay(port));
        port.SetLearn(true);
        break;
    case PrtState::Rerooted:
        port.SetReRoot(false);
        break;
    case PrtState::RootPort:
        port.SetRole(PortRole::Root);
        port.GetSmTimers().SetRrWhile(FwdDelay(port));
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::PortRoleTransitionsDesignatedState(Port& port) noexcept
{
    switch (port.PrtState()) {
    case PrtState::DesignatedPropose:
        goto PRT_DESIGNATED_PORT; // UCT

        PRT_DESIGNATED_PROPOSE:
        port.SetPrtState(PrtState::DesignatedPropose);
        PortRoleTransitionsDesignatedStateAction(port);
        break;
    case PrtState::DesignatedSynced:
        goto PRT_DESIGNATED_PORT; // UCT

        PRT_DESIGNATED_SYNCED:
        port.SetPrtState(PrtState::DesignatedSynced);
        PortRoleTransitionsDesignatedStateAction(port);
        break;
    case PrtState::DesignatedRetired:
        goto PRT_DESIGNATED_PORT; // UCT

        PRT_DESIGNATED_RETIRED:
        port.SetPrtState(PrtState::DesignatedRetired);
        PortRoleTransitionsDesignatedStateAction(port);
        break;
    case PrtState::DesignatedForward:
        goto PRT_DESIGNATED_PORT; // UCT

        PRT_DESIGNATED_FORWARD:
        port.SetPrtState(PrtState::DesignatedForward);
        PortRoleTransitionsDesignatedStateAction(port);
        break;
    case PrtState::DesignatedLearn:
        goto PRT_DESIGNATED_PORT; // UCT

        PRT_DESIGNATED_LEARN:
        port.SetPrtState(PrtState::DesignatedLearn);
        PortRoleTransitionsDesignatedStateAction(port);
        break;
    case PrtState::DesignatedDiscard:
        goto PRT_DESIGNATED_PORT; // UCT

        PRT_DESIGNATED_DISCARD:
        port.SetPrtState(PrtState::DesignatedDiscard);
        PortRoleTransitionsDesignatedStateAction(port);
        break;
    case PrtState::DesignatedPort:
        if (not (port.Selected() && not port.UpdtInfo())) {
            break;
        }

        if (not port.Forward() && not port.Agreed() && not port.Proposing() && not port.OperEdge()) {
            goto PRT_DESIGNATED_PROPOSE;
        }

        if ((not port.Learning() && not port.Forwarding() && not port.Synced())
                 || (port.Agreed() && not port.Synced()) || (port.OperEdge() && not port.Synced())
                 || (port.Sync() && port.Synced())) {
            goto PRT_DESIGNATED_SYNCED;
        }

        if (Timer::TimedOut(port.SmTimers().RrWhile()) && port.ReRoot()) {
            goto PRT_DESIGNATED_RETIRED;
        }

        if (((port.Sync() && not port.Synced()) || (port.ReRoot()
                                                         && (not Timer::TimedOut(port.SmTimers().RrWhile())))
                  || port.Disputed()) && not port.OperEdge() && (port.Learn() || port.Forward())) {
            goto PRT_DESIGNATED_DISCARD;
        }

        if ((Timer::TimedOut(port.SmTimers().FdWhile()) || port.Agreed() || port.OperEdge())
                 && (Timer::TimedOut(port.SmTimers().RrWhile()) || not port.ReRoot()) && not port.Sync()
                 && not port.Learn()) {
            goto PRT_DESIGNATED_LEARN;
        }

        if ((Timer::TimedOut(port.SmTimers().FdWhile()) || port.Agreed() || port.OperEdge())
                 && (Timer::TimedOut(port.SmTimers().RrWhile()) || not port.ReRoot()) && not port.Sync()
                 && (port.Learn() && not port.Forward())) {
            goto PRT_DESIGNATED_FORWARD;
        }

        break;
        PRT_DESIGNATED_PORT:
        port.SetPrtState(PrtState::DesignatedPort);
        PortRoleTransitionsDesignatedStateAction(port);
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::PortRoleTransitionsDesignatedStateAction(Port& port) noexcept
{
    switch (port.PrtState()) {
    case PrtState::DesignatedPropose:
        port.SetProposing(true);
        port.GetSmTimers().SetEdgeDelayWhile(EdgeDelay(port));
        port.SetNewInfo(true);
        break;
    case PrtState::DesignatedSynced:
        port.GetSmTimers().SetRrWhile(0);
        port.SetSynced(true);
        port.SetSync(false);
        break;
    case PrtState::DesignatedRetired:
        port.SetReRoot(false);
        break;
    case PrtState::DesignatedForward:
        port.SetForward(true);
        port.GetSmTimers().SetFdWhile(0);
        port.SetAgreed(port.SendRstp());
        break;
    case PrtState::DesignatedLearn:
        port.SetLearn(true);
        port.GetSmTimers().SetFdWhile(ForwardDelay(port));
        break;
    case PrtState::DesignatedDiscard:
        port.SetDisputed(false);
        port.SetForward(false);
        port.SetLearn(false);
        port.GetSmTimers().SetFdWhile(ForwardDelay(port));
        break;
    case PrtState::DesignatedPort:
        port.SetRole(PortRole::Designated);
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::PortRoleTransitionsAlternateAndBackupState(Port& port) noexcept
{
    switch (port.PrtState()) {
    case PrtState::AlternateProposed:
        goto PRT_ALTERNATE_PORT; // UCT

        PRT_ALTERNATE_PROPOSED:
        port.SetPrtState(PrtState::AlternateProposed);
        PortRoleTransitionsAlternateAndBackupStateAction(port);
        break;
    case PrtState::AlternateAgreed:
        goto PRT_ALTERNATE_PORT; // UCT

        PRT_ALTERNATE_AGREED:
        port.SetPrtState(PrtState::AlternateAgreed);
        PortRoleTransitionsAlternateAndBackupStateAction(port);
        break;
    case PrtState::BlockPort:
        if (not port.Learning() && not port.Forwarding()
                && port.Selected() && not port.UpdtInfo()) {
            goto PRT_ALTERNATE_PORT;
        }

        break;
        /// @todo Make sure that PRT_BLOCK_PORT label is not reachable
//        PRT_BLOCK_PORT:
//        port.SetPrtState(PrtState::BlockPort);
//        PortRoleTransitionsAlternateAndBackupStateAction(port);
//        break;
    case PrtState::BackupPort:
        goto PRT_ALTERNATE_PORT; // UCT

        PRT_BACKUP_PORT:
        port.SetPrtState(PrtState::BackupPort);
        PortRoleTransitionsAlternateAndBackupStateAction(port);
        break;
    case PrtState::AlternatePort:
        if (not (port.Selected() && not port.UpdtInfo())) {
            break;
        }

        if (port.Proposed() && not port.Agree()) {
            goto PRT_ALTERNATE_PROPOSED;
        }

        if ((AllSynced() && not port.Agree()) || (port.Proposed() && port.Agree())) {
            goto PRT_ALTERNATE_AGREED;
        }

        if ((port.SmTimers().FdWhile() != ForwardDelay(port)) || port.Sync() || port.ReRoot()
                 || not port.Synced()) {
            goto PRT_ALTERNATE_PORT;
        }

        if ((port.SmTimers().RbWhile() != 2 * HelloTime(port))
                 && (PortRole::Backup == port.Role())) {
            goto PRT_BACKUP_PORT;
        }

        break;
        PRT_ALTERNATE_PORT:
        port.SetPrtState(PrtState::AlternatePort);
        PortRoleTransitionsAlternateAndBackupStateAction(port);
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::PortRoleTransitionsAlternateAndBackupStateAction(Port& port) noexcept
{
    switch (port.PrtState()) {
    case PrtState::AlternateProposed:
        SetSyncTree();
        port.SetProposed(false);
        break;
    case PrtState::AlternateAgreed:
        port.SetProposed(false);
        port.SetAgree(true);
        port.SetNewInfo(true);
        break;
    case PrtState::BlockPort:
        port.SetRole(port.SelectedRole());
        port.SetForward(false);
        port.SetLearn(false);
        break;
    case PrtState::BackupPort:
        port.GetSmTimers().SetRbWhile(2 * HelloTime(port));
        break;
    case PrtState::AlternatePort:
        port.GetSmTimers().SetFdWhile(FwdDelay(port));
        port.SetSynced(true);
        port.GetSmTimers().SetRrWhile(0);
        port.SetSync(false);
        port.SetReRoot(false);
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::PortStateTransition(Port& port) noexcept
{
    switch (port.PstState()) {
    case PstState::Discarding:
        if (port.Learn()) {
            goto PST_LEARNING;
        }

        break;
        PST_DISCARDING:
        port.SetPstState(PstState::Discarding);
        PortStateTransitionAction(port);
        break;
    case PstState::Learning:
        if (port.Forward()) {
            goto PST_FORWARDING;
        }

        if (not port.Learn()) {
            goto PST_DISCARDING;
        }

        break;
        PST_LEARNING:
        port.SetPstState(PstState::Learning);
        PortStateTransitionAction(port);
        break;
    case PstState::Forwarding:
        if (not port.Forward()) {
            goto PST_DISCARDING;
        }

        break;
        PST_FORWARDING:
        port.SetPstState(PstState::Forwarding);
        PortStateTransitionAction(port);
        break;
    case PstState::Begin:
        if (_bridge.Begin()) {
            goto PST_DISCARDING;
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::PortStateTransitionAction(Port& port) noexcept
{
    switch (port.PstState()) {
    case PstState::Discarding:
        DisableLearning(port);
        port.SetLearning(false);
        DisableForwarding(port);
        port.SetForwarding(false);
        break;
    case PstState::Learning:
        EnableLearning(port);
        port.SetLearning(true);
        // 17.19.1
        port.SetAgeingTime(Bridge::AgeingTime);
        break;
    case PstState::Forwarding:
        EnableForwarding(port);
        port.SetForwarding(true);
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::TopologyChange(Port& port) noexcept
{
    switch (port.TcmState()) {
    case TcmState::Inactive:
        if (port.Learn() && not port.FdbFlush()) {
            goto TCM_LEARNING;
        }

        break;
        TCM_INACTIVE:
        port.SetTcmState(TcmState::Inactive);
        TopologyChangeAction(port);
        break;
    case TcmState::Learning:
        if (((PortRole::Root == port.Role()) || (PortRole::Designated == port.Role()))
                && port.Forward() && not port.OperEdge()) {
            goto TCM_DETECTED;
        }

        if (port.RcvdTc() || port.RcvdTcn() || port.RcvdTcAck() || port.TcProp()) {
            goto TCM_LEARNING;
        }

        if ((PortRole::Root != port.Role()) && (PortRole::Designated != port.Role()) &&
                 not (port.Learn() || port.Learning()) &&
                 not (port.RcvdTc() || port.RcvdTcn() || port.RcvdTcAck() || port.TcProp())) {
            goto TCM_INACTIVE;
        }

        break;
        TCM_LEARNING:
        port.SetTcmState(TcmState::Learning);
        TopologyChangeAction(port);
        break;
    case TcmState::Detected:
        goto TCM_ACTIVE; // UCT

        TCM_DETECTED:
        port.SetTcmState(TcmState::Detected);
        TopologyChangeAction(port);
        break;
    case TcmState::NotifiedTcn:
        goto TCM_NOTIFIED_TC; // UCT

        TCM_NOTIFIED_TCN:
        port.SetTcmState(TcmState::NotifiedTcn);
        TopologyChangeAction(port);
        break;
    case TcmState::NotifiedTc:
        goto TCM_ACTIVE; // UCT

        TCM_NOTIFIED_TC:
        port.SetTcmState(TcmState::NotifiedTc);
        TopologyChangeAction(port);
        break;
    case TcmState::Propagating:
        goto TCM_ACTIVE; // UCT

        TCM_PROPAGATING:
        port.SetTcmState(TcmState::Propagating);
        TopologyChangeAction(port);
        break;
    case TcmState::Acknowledged:
        goto TCM_ACTIVE; // UCT

        TCM_ACKNOWLEDGED:
        port.SetTcmState(TcmState::Acknowledged);
        TopologyChangeAction(port);
        break;
    case TcmState::Active:
        if (((PortRole::Root != port.Role()) && (PortRole::Designated != port.Role()))
                || port.OperEdge()) {
            goto TCM_LEARNING;
        }

        if (port.RcvdTcn()) {
            goto TCM_NOTIFIED_TCN;
        }

        if (port.RcvdTc()) {
            goto TCM_NOTIFIED_TC;
        }

        if (port.TcProp() && not port.OperEdge()) {
            goto TCM_PROPAGATING;
        }

        if (port.RcvdTcAck()) {
            goto TCM_ACKNOWLEDGED;
        }

        break;
        TCM_ACTIVE:
        port.SetTcmState(TcmState::Active);
        TopologyChangeAction(port);
        break;
    case TcmState::Begin:
        if (_bridge.Begin()) {
            goto TCM_INACTIVE;
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid state\n";
        break;
    }
}

void SpanningTreeProtocol::TopologyChangeAction(Port& port) noexcept
{
    switch (port.TcmState()) {
    case TcmState::Inactive:
        /// @todo Do we need really here to flush entries data base?
        if (Failed(_interface.PortFdbFlush(port.PortId().PortNum()))) {
            std::cerr << __PRETTY_FUNCTION__ << "Failed to flush filtering database entries\n";
        }

        port.SetFdbFlush(true);
        port.GetSmTimers().SetTcWhile(0);
        port.SetTcAck(false);
        // 17.19.1
        port.SetAgeingTime(StpVersion() ? FwdDelay(port) : Bridge::AgeingTime);
        break;
    case TcmState::Learning:
        port.SetRcvdTc(false);
        port.SetRcvdTcn(false);
        port.SetRcvdTcAck(false);
        port.SetTcProp(false);
        break;
    case TcmState::Detected:
        NewTcWhile(port);
        SetTcPropTree(port);
        port.SetNewInfo(true);
        break;
    case TcmState::NotifiedTcn:
        NewTcWhile(port);
        break;
    case TcmState::NotifiedTc:
        port.SetRcvdTc(false);
        port.SetRcvdTcn(false);

        if (PortRole::Designated == port.Role()) {
            port.SetTcAck(true);
        }

        SetTcPropTree(port);
        break;
    case TcmState::Propagating:
        NewTcWhile(port);

        if (Failed(_interface.PortFdbFlush(port.PortId().PortNum()))) {
            std::cerr << __PRETTY_FUNCTION__ << "Failed to flush filtering database entries\n";
        }

        port.SetFdbFlush(true);
        port.SetTcProp(false);
        // 17.19.1
        port.SetAgeingTime(StpVersion() ? FwdDelay(port) : Bridge::AgeingTime);
        break;
    case TcmState::Acknowledged:
        port.GetSmTimers().SetTcWhile(0);
        port.SetRcvdTcAck(false);
        break;
    case TcmState::Active:

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << " reached invalid action\n";
        break;
    }
}

void SpanningTreeProtocol::UpdtBpduVersion(Port& port) noexcept
{
    if (Bpdu::Type::Rst == port.RxBpdu().BpduType()) {
        port.SetRcvdStp(false);
        port.SetRcvdRstp(true);
    }
    else {
        port.SetRcvdRstp(false);
        port.SetRcvdStp(true);
    }
}

bool SpanningTreeProtocol::AllSynced() noexcept
{
    bool allSynced = true;

    for (const Port& portMap : _ports) {
        if (not (portMap.Selected() && portMap.Role() == portMap.SelectedRole()
                 && (portMap.Synced() || (portMap.Role() == PortRole::Root)))) {
            allSynced = false;
            break;
        }
    }

    return allSynced;
}

bool SpanningTreeProtocol::ReRooted(const Port& port) const noexcept
{
    bool result = true;

    for (Port& otherPort : _ports) {
        if ((otherPort.PortId().PortNum() != port.PortId().PortNum())
                && (not Timer::TimedOut(port.SmTimers().RrWhile()))) {
            result = false;
            break;
        }
    }

    return result;
}

bool SpanningTreeProtocol::BetterOrSameInfo(const Port& port, const Port::Info newInfoIs) const noexcept
{
    bool result = false;

    if (Port::Info::Received == newInfoIs && Port::Info::Received == port.InfoIs()
            && ((port.PortPriority() < port.MsgPriority())
                || (port.PortPriority() == port.MsgPriority()))) {
        result = true;
    }
    else if (Port::Info::Mine == newInfoIs && Port::Info::Mine == port.InfoIs()
             && ((port.PortPriority() < port.DesignatedPriority())
                 || (port.PortPriority() == port.DesignatedPriority()))) {
        result = true;
    }

    return result;
}

void SpanningTreeProtocol::DisableForwarding(const Port& port) const noexcept
{
    _interface.PortForwarding(port.PortId().PortNum(), false);
}

void SpanningTreeProtocol::DisableLearning(const Port& port) const noexcept
{
    _interface.PortLearning(port.PortId().PortNum(), false);
}

void SpanningTreeProtocol::EnableForwarding(const Port& port) const noexcept
{
    _interface.PortForwarding(port.PortId().PortNum(), true);
}

void SpanningTreeProtocol::EnableLearning(const Port& port) const noexcept
{
    _interface.PortLearning(port.PortId().PortNum(), true);
}

void SpanningTreeProtocol::NewTcWhile(Port& port) noexcept
{
    if (0 == port.SmTimers().TcWhile()) {
        if (port.SendRstp()) {
            port.GetSmTimers().SetTcWhile(port.PortTimes().HelloTime() + 1); // plus one second
            port.SetNewInfo(true);
        }
        else {
            port.GetSmTimers().SetTcWhile(_bridge.RootTimes().MaxAge()
                                          + _bridge.RootTimes().ForwardDelay());
        }
    }
}

enum Port::RcvdInfo SpanningTreeProtocol::RcvInfo(Port& port) noexcept
{
    if ((u8)Bpdu::Type::Invalid != port.RxBpdu().BpduType()) {
        // Decodes the message priority and timer values from the received BPDU storing them in the
        // msgPriority and msgTimes variables.
        port.GetMsgPriority().SetRootBridgeId(BridgeId(port.RxBpdu().RootIdentifier()));
        port.GetMsgPriority().GetRootPathCost().SetPathCost(port.RxBpdu().RootPathCost());
        port.GetMsgPriority().SetDesignatedBridgeId(BridgeId(port.RxBpdu().BridgeIdentifier()));
        port.GetMsgPriority().SetDesignatedPortId(PortId(port.RxBpdu().PortIdentifier()));

        port.GetMsgTimes().SetMessageAge(port.RxBpdu().MessageAge());
        port.GetMsgTimes().SetMaxAge(port.RxBpdu().MaxAge());
        port.GetMsgTimes().SetHelloTime(port.RxBpdu().HelloTime());
        port.GetMsgTimes().SetForwardDelay(port.RxBpdu().ForwardDelay());
    }

    PortRole encodedPortRole = port.RxBpdu().PortRoleFlag();
    enum Port::RcvdInfo result = Port::RcvdInfo::OtherInfo;

    /// @note A Configuration BPDU explicitly conveys a Designated Port Role.
    if (encodedPortRole != PortRole::Designated) {
        if (port.RcvdBpdu() && (Bpdu::Type::Config == port.RxBpdu().BpduType())) {
            encodedPortRole = PortRole::Designated;
        }
    }

    switch (encodedPortRole) {
    case PortRole::Designated: {
        if (port.PortPriority() < port.MsgPriority()) {
            /// @note 17.21.8 a1)
            result = Port::RcvdInfo::SuperiorDesignatedInfo;
            break;
        }

        if (port.MsgPriority() == port.PortPriority()) {
            if ((port.MsgTimes().ForwardDelay() != port.PortTimes().ForwardDelay())
                    || (port.MsgTimes().HelloTime() != port.PortTimes().HelloTime())
                    || (port.MsgTimes().MaxAge() != port.PortTimes().MaxAge())
                    || (port.MsgTimes().MessageAge() != port.PortTimes().MessageAge())) {
                /// @note 17.21.8 a2)
                result = Port::RcvdInfo::SuperiorDesignatedInfo;
            }
            /// @todo Check if correct is apply this condition
            // According to clause 13.26.6 b2) of 802.1Q-2005
            else /* if (port.InfoIs() == Port::Info::Received) */ {
                /// @note 17.21.8 b)
                result = Port::RcvdInfo::RepeatedDesignatedInfo;
            }

            break;
        }

        /// @note 17.21.8 c)
        result = Port::RcvdInfo::InferiorDesignatedInfo;
        break;

//        if (not (port.PortPriority() < port.MsgPriority())) {
//            if (port.MsgPriority() == port.PortPriority()) {
//                if ((port.MsgTimes().ForwardDelay() != port.PortTimes().ForwardDelay())
//                        || (port.MsgTimes().HelloTime() != port.PortTimes().HelloTime())
//                        || (port.MsgTimes().MaxAge() != port.PortTimes().MaxAge())
//                        || (port.MsgTimes().MessageAge() != port.PortTimes().MessageAge())) {
//                    /// @note 17.21.8 a2)
//                    result = Port::RcvdInfo::SuperiorDesignatedInfo;
//                }
//                else if (((port.MsgTimes().ForwardDelay() == port.PortTimes().ForwardDelay())
//                         || (port.MsgTimes().HelloTime() == port.PortTimes().HelloTime())
//                         || (port.MsgTimes().MaxAge() == port.PortTimes().MaxAge())
//                         || (port.MsgTimes().MessageAge() == port.PortTimes().MessageAge()))
//                         // According to clause 13.26.6 of 802.1Q-2005
//                          && (port.InfoIs() == Port::Info::Received)) {
//                    /// @note 17.21.8 b)
//                    result = Port::RcvdInfo::RepeatedDesignatedInfo;
//                }
//            }
//            else {
//                /// @note 17.21.8 c)
//                result = Port::RcvdInfo::InferiorDesignatedInfo;
//            }
//        }
//        else { // port.PortPriority() > port.MsgPriority()
//            /// @note 17.21.8 a1)
//            result = Port::RcvdInfo::SuperiorDesignatedInfo;
//        }

//        break;
    } // PortRole::Designated
    case PortRole::Root:
    case PortRole::Alternate:
    case PortRole::Backup:
        /// @note 17.21.8 d)
        result = Port::RcvdInfo::InferiorRootAlternateInfo;
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << "Invalid encoded role port\n";
    }

    return result;
}

void SpanningTreeProtocol::RecordAgreement(Port& port) noexcept
{
    if (RstpVersion() && port.operPointToPointMAC
            && (port.RcvdBpdu() && ((Bpdu::Type::Config == port.RxBpdu().BpduType())
                                    && port.RxBpdu().AgreementFlag()))) {
        port.SetAgreed(true);
        port.SetProposing(false);
    }
    else {
        port.SetAgreed(false);
    }
}

void SpanningTreeProtocol::RecordDispute(Port& port) noexcept
{
    if (port.RcvdBpdu() && (Bpdu::Type::Rst == port.RxBpdu().BpduType()
                            && port.RxBpdu().LearnigFlag())) {
        port.SetAgreed(true);
        port.SetProposing(false);
    }
}

void SpanningTreeProtocol::RecordProposal(Port& port) noexcept
{
    if ((Bpdu::Type::Config == port.RxBpdu().BpduType())
            && (port.RxBpdu().PortRoleFlag() == PortRole::Designated)
            && port.RxBpdu().ProposalFlag()) {
        port.SetProposed(true);
    }
}

void SpanningTreeProtocol::RecordPriority(Port& port) noexcept
{
    port.SetPortPriority(port.MsgPriority());
}

void SpanningTreeProtocol::RecordTimes(Port &port) noexcept
{
    port.GetPortTimes().SetMessageAge(port.MsgTimes().MessageAge());
    port.GetPortTimes().SetMaxAge(port.MsgTimes().MaxAge());
    port.GetPortTimes().SetForwardDelay(port.MsgTimes().ForwardDelay());
    // 1 is minimum compatability range value of Hello Time parameter
    port.GetPortTimes().SetHelloTime(port.MsgTimes().HelloTime() > 1 ? port.MsgTimes().HelloTime() : 1);
}

void SpanningTreeProtocol::SetSyncTree() noexcept
{
    for (Port& port : _ports) {
        port.SetSync(true);
    }
}

void SpanningTreeProtocol::SetReRootTree() noexcept
{
    for (Port& port : _ports) {
        port.SetReRoot(true);
    }
}

void SpanningTreeProtocol::SetSelectedTree() noexcept
{
    bool reselect = false;

    for (const Port& port : _ports) {
        if (port.Reselect()) {
            reselect = true;
            break;
        }
    }

    if (not reselect) {
        for (Port& port : _ports) {
            port.SetSelected(true);
        }
    }
}

void SpanningTreeProtocol::SetTcFlags(Port& port) noexcept
{
    if (not port.RcvdBpdu()) {
        return;
    }

    switch (static_cast<Bpdu::Type>(port.RxBpdu().BpduType())) {
    case Bpdu::Type::Config:
    case Bpdu::Type::Rst: {
        if (port.RxBpdu().TcFlag()) {
            port.SetRcvdTc(true);
        }

        if (port.RxBpdu().TcAckFlag()) {
            port.SetRcvdTcAck(true);
        }

        break;
    }
    case Bpdu::Type::Tcn:
        if (port.RcvdBpdu() && (Bpdu::Type::Tcn == port.RxBpdu().BpduType())) {
            port.SetRcvdTcn(true);
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << "Invalid BPDU type\n";
        break;
    }
}

void SpanningTreeProtocol::SetTcPropTree(const Port& port) const noexcept
{
    for (Port& otherPort : _ports) {
        if (otherPort.PortId().PortNum() != port.PortId().PortNum()) {
            otherPort.SetTcProp(true);
        }
    }
}

void SpanningTreeProtocol::TxConfig(const Port& port) const
{
    if (port.DesignatedTimes().MessageAge() >= port.DesignatedTimes().MaxAge()) {
        std::cerr << __PRETTY_FUNCTION__ << "Invalid message age\n";
        return;
    }

    ByteStream bpduStream;
    Bpdu bpdu;
    u16 time;

    bpdu.SetProtocolIdentifier(static_cast<u16>(Bpdu::ProtocolIdentifier::Config));
    bpdu.SetProtocolVersionIdentifier(static_cast<u8>(Bpdu::ProtocolVersionIdentifier::Config));
    bpdu.SetBpduType(static_cast<u8>(Bpdu::Type::Config));

    if (not Timer::TimedOut(port.SmTimers().TcWhile())) {
        bpdu.SetTcFlag();
    }

    if (port.TcAck()) {
        bpdu.SetTcAckFlag();
    }

    bpdu.SetRootIdentifier(port.DesignatedPriority().RootBridgeId().ConvertToBpduData());
    bpdu.SetRootPathCost(port.DesignatedPriority().RootPathCost().Value());
    bpdu.SetBridgeIdentifier(port.DesignatedPriority().DesignatedBridgeId().ConvertToBpduData());
    bpdu.SetPortIdentifier(port.DesignatedPriority().DesignatedPortId().ConvertToBpduData());

    /// @todo We don't wanna convert endianess here - it is responsible of Bpdu class
    time = static_cast<u8>(port.DesignatedTimes().HelloTime() / (u16)ShiftOctet::CpuLeastSignificant1st);
    time += static_cast<u8>(port.DesignatedTimes().HelloTime() / (u16)ShiftOctet::CpuLeastSignificant2nd) * 0x100;
    bpdu.SetHelloTime(time);

    time = static_cast<u8>(port.DesignatedTimes().MaxAge() / (u16)ShiftOctet::CpuLeastSignificant1st);
    time += static_cast<u8>(port.DesignatedTimes().MaxAge() / (u16)ShiftOctet::CpuLeastSignificant2nd) * 0x100;
    bpdu.SetMaxAge(time);

    time = static_cast<u8>(port.DesignatedTimes().MessageAge() / (u16)ShiftOctet::CpuLeastSignificant1st);
    time += static_cast<u8>(port.DesignatedTimes().MessageAge() / (u16)ShiftOctet::CpuLeastSignificant2nd) * 0x100;
    bpdu.SetMessageAge(time);

    time = static_cast<u8>(port.DesignatedTimes().ForwardDelay() / (u16)ShiftOctet::CpuLeastSignificant1st);
    time += static_cast<u8>(port.DesignatedTimes().ForwardDelay() / (u16)ShiftOctet::CpuLeastSignificant2nd) * 0x100;
    bpdu.SetForwardDelay(time);

    if (Failed(bpdu.Encode(bpduStream))) {
        std::cerr << __PRETTY_FUNCTION__ << "Failed to encode BPDU unit data\n";
    }
    else {
        _interface.PortSendBpdu(port.PortId().PortNum(), bpduStream);
    }
}

void SpanningTreeProtocol::TxRstp(const Port& port) const
{
    if (port.DesignatedTimes().MessageAge() >= port.DesignatedTimes().MaxAge()) {
        std::cerr << __PRETTY_FUNCTION__ << "Invalid message age\n";
        return;
    }

    ByteStream bpduStream;
    Bpdu bpdu;

    bpdu.SetProtocolIdentifier(static_cast<u16>(Bpdu::ProtocolIdentifier::Rst));
    bpdu.SetProtocolVersionIdentifier(static_cast<u8>(Bpdu::ProtocolVersionIdentifier::Rst));
    bpdu.SetBpduType(static_cast<u8>(Bpdu::Type::Rst));
    bpdu.SetPortRoleFlag(port.Role());

    if (port.Agree()) {
        bpdu.SetAgreementFlag();
    }

    if (port.Proposing()) {
        bpdu.SetProposalFlag();
    }

    if (not Timer::TimedOut(port.SmTimers().TcWhile())) {
        bpdu.SetTcFlag();
    }

    if (port.Learning()) {
        bpdu.LearnigFlag();
    }

    if (port.Forwarding()) {
        bpdu.SetForwardingFlag();
    }

    bpdu.SetRootIdentifier(port.DesignatedPriority().RootBridgeId().ConvertToBpduData());
    bpdu.SetRootPathCost(port.DesignatedPriority().RootPathCost().Value());
    bpdu.SetBridgeIdentifier(port.DesignatedPriority().DesignatedBridgeId().ConvertToBpduData());
    bpdu.SetPortIdentifier(port.DesignatedPriority().DesignatedPortId().ConvertToBpduData());

    bpdu.SetHelloTime(port.DesignatedTimes().HelloTime());
    bpdu.SetMaxAge(port.DesignatedTimes().MaxAge());
    bpdu.SetMessageAge(port.DesignatedTimes().MessageAge());
    bpdu.SetForwardDelay(port.DesignatedTimes().ForwardDelay());

    bpdu.SetVersion1Length((u8)Bpdu::Version1Length::Rst);

    if (Failed(bpdu.Encode(bpduStream))) {
        std::cerr << __PRETTY_FUNCTION__ << "Failed to encode BPDU unit data\n";
    }
    else {
        _interface.PortSendBpdu(port.PortId().PortNum(), bpduStream);
    }
}

void SpanningTreeProtocol::TxTcn(const Port& port) const
{
    ByteStream bpduStream;
    Bpdu bpdu;

    bpdu.SetProtocolIdentifier(static_cast<u16>(Bpdu::ProtocolIdentifier::Tcn));
    bpdu.SetProtocolVersionIdentifier(static_cast<u8>(Bpdu::ProtocolVersionIdentifier::Tcn));
    bpdu.SetBpduType(static_cast<u8>(Bpdu::Type::Tcn));

    if (Failed(bpdu.Encode(bpduStream))) {
        std::cerr << __PRETTY_FUNCTION__ << "Failed to encode BPDU unit data\n";
    }
    else {
        _interface.PortSendBpdu(port.PortId().PortNum(), bpduStream);
    }
}

void SpanningTreeProtocol::UpdtRcvdInfoWhile(Port& port) noexcept
{
    port.GetSmTimers().SetRcvdInfoWhile((port.PortTimes().MessageAge() + 1) <= port.PortTimes().MaxAge() ?
                                            3 * port.PortTimes().HelloTime() : 0);
}

void SpanningTreeProtocol::UpdtRoleDisabledTree() noexcept
{
    for (Port& port : _ports) {
        port.SetSelectedRole(PortRole::Disabled);
    }
}

/// @todo Review based on clause 17.6
void SpanningTreeProtocol::UpdtRolesTree() noexcept
{
    struct RootPathPriority {
        PriorityVector priorityVector;
        PortId portId;
        Time times;
    };

    // The Bridge’s root priority vector (rootPriority plus rootPortId; 17.18.6, 17.18.5), chosen
    // as the best of the set of priority vectors
    RootPathPriority bestRootPriorityVector {
        _bridge.RootPriority(), _bridge.RootPortId(), _bridge.RootTimes()
    };

    for (Port& port : _ports) {
        if (not (Port::Info::Received == port.InfoIs())) {
            continue;
        }

        RootPathPriority rootPathPriority {
            port.PortPriority(), port.PortId(), port.PortTimes()
        };
        // a & 17.6
        rootPathPriority.priorityVector.GetRootPathCost() += port.PortPathCost();

        // All the calculated root path priority vectors whose DesignatedBridgeID Bridge Address
        // component is not equal to that component of the Bridge’s own bridge priority vector
        if (rootPathPriority.priorityVector.DesignatedBridgeId().Address()
                == _bridge.BridgePriority().DesignatedBridgeId().Address()) {
            continue;
        }
        // The Bridge’s root priority vector, chosen as the best of the set of priority
        // vectors comprising the Bridge’s own bridge priority vector.
        if ((bestRootPriorityVector.priorityVector < rootPathPriority.priorityVector)
                || ((bestRootPriorityVector.priorityVector == rootPathPriority.priorityVector)
                    && (bestRootPriorityVector.portId < rootPathPriority.portId))) {
            // b)
            bestRootPriorityVector.priorityVector = rootPathPriority.priorityVector;
            bestRootPriorityVector.portId = rootPathPriority.portId;
            bestRootPriorityVector.times = rootPathPriority.times;
        }
    }

    if (bestRootPriorityVector.priorityVector == _bridge.BridgePriority()) {
        // c1) the chosen root priority vector is the bridge priority vector
        _bridge.SetRootTimes(_bridge.BridgeTimes());
    }
    else {
        // c2)
        /// @todo Confirm that this is propoer place to update rootPriority and rootPortId components
        _bridge.SetRootPriority(bestRootPriorityVector.priorityVector);
        _bridge.SetRootPortId(bestRootPriorityVector.portId);

        _bridge.SetRootTimes(bestRootPriorityVector.times);
        _bridge.GetRootTimes().SetMessageAge(_bridge.RootTimes().MessageAge() + 1);
    }

    for (Port& port : _ports) {
        // d)
        port.SetDesignatedPriority(_bridge.RootPriority());
        port.GetDesignatedPriority().SetDesignatedBridgeId(_bridge.BridgeIdentifier());
        port.GetDesignatedPriority().SetDesignatedPortId(port.PortId());
        port.SetDesignatedTimes(_bridge.RootTimes());
        // e)
        port.GetDesignatedTimes().SetHelloTime(_bridge.BridgeTimes().HelloTime());
    }

    for (Port& port : _ports) {
        switch (port.InfoIs()) {
        case Port::Info::Disabled: {
            // f)
            port.SetSelectedRole(PortRole::Disabled);
            break;
        }
        case Port::Info::Aged: {
            // g)
            port.SetUpdtInfo(true);
            port.SetSelectedRole(PortRole::Designated);
            break;
        }
        /// @todo If the port priority vector was derived from another port on the Bridge
        case Port::Info::Mine: {
            // h)
            port.SetSelectedRole(PortRole::Designated);

            if ((not (port.PortPriority() == port.DesignatedPriority()))
                    || (not (port.PortTimes() == _bridge.RootTimes()))) {
                port.SetUpdtInfo(true);
            }

            break;
        }
        case Port::Info::Received: {
            if (_bridge.RootPortId().PortNum() == port.PortId().PortNum()) {
                // i)
                port.SetSelectedRole(PortRole::Root);
                port.SetUpdtInfo(false);
            }
            else if (not (port.PortPriority() < port.DesignatedPriority())) {
                bool reflected = false;

                /// @todo Confirm that comparing components designatedBridgeId and designatedPortId
                /// are fine without comparing their associated priorities
                for (const Port& otherPort : _ports) {
                    if ((otherPort.PortId().PortNum() != port.PortId().PortNum())
                            && (otherPort.PortPriority().DesignatedBridgeId()
                                == port.PortPriority().DesignatedBridgeId())
                            && (otherPort.PortPriority().DesignatedPortId()
                                == port.PortPriority().DesignatedPortId())) {
                        reflected = true;
                        break;
                    }
                }

                if (not reflected) {
                    // j)
                    port.SetSelectedRole(PortRole::Alternate);
                    port.SetUpdtInfo(false);
                }
                else {
                    // k)
                    port.SetSelectedRole(PortRole::Backup);
                    port.SetUpdtInfo(false);
                }
            }
            else {
                // l)
                port.SetSelectedRole(PortRole::Designated);
                port.SetUpdtInfo(true);
            }

            break;
        }
        }
    }
}

} // namespace Rstp
