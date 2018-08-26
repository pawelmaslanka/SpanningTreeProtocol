// This project's headers
#include "stp/sm_procedures.hpp"
// Dependencies
#include "stp/bpdu.hpp"
#include "stp/sm_conditions.hpp"
#include "stp/time.hpp"

// C++ Standard Library
#include <iostream>

struct RootPathPriority {
    Stp::PortId portId;
    Stp::PriorityVector priorityVector;
    Stp::Time times;
};

namespace Stp {
namespace SmProcedures {

bool AllSynced(Bridge& bridge) noexcept {
    for (const auto& portMapIt : bridge.GetAllPorts()) {
        const Port& port { *(portMapIt.second) };
        if (not port.Selected()) {
            return false;
        }
        else if (port.Role() != port.SelectedRole()) {
            return false;
        }

        if (not port.Synced()) {
            if (port.Role() != PortRole::Root) {
                return false;
            }
        }
    }

    return true;
}

bool BetterOrSameInfo(Port& port, const Port::Info newInfoIs) noexcept {
    bool result = false;
    if (Port::Info::Received == newInfoIs && Port::Info::Received == port.InfoIs()) {
        if (port.PortPriority() < port.MsgPriority()) {
            result = true;
        }
        else if (port.PortPriority() == port.MsgPriority()) {
            result = true;
        }
    }
    else if (Port::Info::Mine == newInfoIs && Port::Info::Mine == port.InfoIs()) {
        if (port.PortPriority() < port.DesignatedPriority()) {
            result = true;
        }
        else if (port.PortPriority() == port.DesignatedPriority()) {
            result = true;
        }
    }

    return result;
}

void ClearReselectTree(Bridge& bridge) noexcept {
    for (auto& portMapIt : bridge.GetAllPorts()) {
        portMapIt.second->SetReselect(false);
    }
}

void FlushFdb(Bridge& bridge, const Port& port) noexcept {
    if (Failed(bridge.FlushFdb(port.PortId().PortNum()))) {
        /// @todo Log failed action
    }
}

u16 MaxAge(const Port& port) noexcept {
    return port.DesignatedTimes().MaxAge();
}

void NewTcWhile(const Bridge& bridge, Port& port) noexcept {
    if (0 == port.SmTimersInstance().TcWhile()) {
        if (port.SendRstp()) {
            port.SmTimersInstance().SetTcWhile(port.PortTimes().HelloTime() + 1); // plus one second
            port.SetNewInfo(true);
        }
        else {
            port.SmTimersInstance().SetTcWhile(bridge.RootTimes().MaxAge()
                                          + bridge.RootTimes().ForwardDelay());
        }
    }
}

void RecordAgreement(Bridge& bridge, Port& port) noexcept {
    bool agreed = true;
    if (not SmConditions::RstpVersion(bridge)) {
        agreed = false;
    }
    else if (not port.operPointToPointMAC) {
        agreed = false;
    }
    else if (not port.RcvdBpdu()) {
        agreed = false;
    }
    else if (not (Bpdu::Type::Config == port.RxBpdu().BpduType())) {
        agreed = false;
    }
    else if (not port.RxBpdu().AgreementFlag()) {
        agreed = false;
    }

    if (agreed) {
        port.SetAgreed(true);
        port.SetProposing(false);
    }
    else {
        port.SetAgreed(false);
    }
}


void RecordDispute(Port& port) noexcept {
    if (not port.RcvdBpdu()) {
        return;
    }
    else if (Bpdu::Type::Rst == port.RxBpdu().BpduType()) {
        return;
    }
    else if (not port.RxBpdu().LearnigFlag()) {
        return;
    }

    port.SetAgreed(true);
    port.SetProposing(false);
}

void RecordPriority(Port& port) noexcept {
    port.SetPortPriority(port.MsgPriority());
}

void RecordProposal(Port& port) noexcept {
    if (not (Bpdu::Type::Config == port.RxBpdu().BpduType())) {
        return;
    }
    else if (not (PortRole::Designated == port.RxBpdu().PortRoleFlag())) {
        return;
    }
    else if (not port.RxBpdu().ProposalFlag()) {
        return;
    }

    port.SetProposed(true);
}

void RecordTimes(Port& port) noexcept {
    port.GetPortTimes().SetMessageAge(port.MsgTimes().MessageAge());
    port.GetPortTimes().SetMaxAge(port.MsgTimes().MaxAge());
    port.GetPortTimes().SetForwardDelay(port.MsgTimes().ForwardDelay());
    // 1 is minimum compatability range value of Hello Time parameter
    const u16 helloTime = port.MsgTimes().HelloTime() > 1 ? port.MsgTimes().HelloTime() : 1;
    port.GetPortTimes().SetHelloTime(helloTime);
}

void SetReRootTree(Bridge& bridge) noexcept {
    for (auto& portMapIt : bridge.GetAllPorts()) {
        portMapIt.second->SetReRoot(true);
    }
}

void SetSelectedTree(Bridge& bridge) noexcept {
    bool reselect = false;

    for (const auto& portMapIt : bridge.GetAllPorts()) {
        if (portMapIt.second->Reselect()) {
            reselect = true;
            break;
        }
    }

    if (not reselect) {
        for (auto& portMapIt : bridge.GetAllPorts()) {
            portMapIt.second->SetSelected(true);
        }
    }
}

void SetSyncTree(Bridge& bridge) noexcept {
    for (auto& portMapIt : bridge.GetAllPorts()) {
        portMapIt.second->SetSync(true);
    }
}

void SetTcFlags(Port& port) noexcept {
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
        if (port.RcvdBpdu()) {
            if (Bpdu::Type::Tcn == port.RxBpdu().BpduType()) {
                port.SetRcvdTcn(true);
            }
        }

        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << "Invalid BPDU type\n";
        break;
    }
}

void SetTcPropTree(Bridge& bridge, const Port& port) noexcept {
    for (auto& portMapIt : bridge.GetAllPorts()) {
        Port& otherPort = *(portMapIt.second);
        if (otherPort.PortId().PortNum() != port.PortId().PortNum()) {
            otherPort.SetTcProp(true);
        }
    }
}

void TxConfig(Bridge& bridge, Port& port) {
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

    if (not SmTimers::TimedOut(port.SmTimersInstance().TcWhile())) {
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
    time = static_cast<u8>(port.DesignatedTimes().HelloTime() / +ShiftOctet::CpuLeastSignificant1st);
    time += static_cast<u8>(port.DesignatedTimes().HelloTime() / +ShiftOctet::CpuLeastSignificant2nd) * 0x100;
    bpdu.SetHelloTime(time);

    time = static_cast<u8>(port.DesignatedTimes().MaxAge() / +ShiftOctet::CpuLeastSignificant1st);
    time += static_cast<u8>(port.DesignatedTimes().MaxAge() / +ShiftOctet::CpuLeastSignificant2nd) * 0x100;
    bpdu.SetMaxAge(time);

    time = static_cast<u8>(port.DesignatedTimes().MessageAge() / +ShiftOctet::CpuLeastSignificant1st);
    time += static_cast<u8>(port.DesignatedTimes().MessageAge() / +ShiftOctet::CpuLeastSignificant2nd) * 0x100;
    bpdu.SetMessageAge(time);

    time = static_cast<u8>(port.DesignatedTimes().ForwardDelay() / +ShiftOctet::CpuLeastSignificant1st);
    time += static_cast<u8>(port.DesignatedTimes().ForwardDelay() / +ShiftOctet::CpuLeastSignificant2nd) * 0x100;
    bpdu.SetForwardDelay(time);

    if (Failed(bpdu.Encode(bpduStream))) {
        std::cerr << __PRETTY_FUNCTION__ << "Failed to encode BPDU unit data\n";
    }
    else {
        bridge.SendOutBpdu(port.PortId().PortNum(), bpduStream);
    }
}

void TxRstp(Bridge& bridge, Port& port) {
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

    if (not SmTimers::TimedOut(port.SmTimersInstance().TcWhile())) {
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

    bpdu.SetVersion1Length(+Bpdu::Version1Length::Rst);

    if (Failed(bpdu.Encode(bpduStream))) {
        std::cerr << __PRETTY_FUNCTION__ << "Failed to encode BPDU unit data\n";
    }
    else {
        bridge.SendOutBpdu(port.PortId().PortNum(), bpduStream);
    }
}

void TxTcn(Bridge& bridge, Port& port) {
    ByteStream bpduStream;
    Bpdu bpdu;

    bpdu.SetProtocolIdentifier(static_cast<u16>(Bpdu::ProtocolIdentifier::Tcn));
    bpdu.SetProtocolVersionIdentifier(static_cast<u8>(Bpdu::ProtocolVersionIdentifier::Tcn));
    bpdu.SetBpduType(static_cast<u8>(Bpdu::Type::Tcn));

    if (Failed(bpdu.Encode(bpduStream))) {
        std::cerr << __PRETTY_FUNCTION__ << "Failed to encode BPDU unit data\n";
    }
    else {
        bridge.SendOutBpdu(port.PortId().PortNum(), bpduStream);
    }
}

void UpdtBpduVersion(Port& port) noexcept {
    if (Bpdu::Type::Rst == port.RxBpdu().BpduType()) {
        port.SetRcvdStp(false);
        port.SetRcvdRstp(true);
    }
    else {
        port.SetRcvdRstp(false);
        port.SetRcvdStp(true);
    }
}

void UpdtRcvdInfoWhile(Port& port) noexcept {
    u16 rcvdInfoWhile = {};
    if ((port.PortTimes().MessageAge() + 1) <= port.PortTimes().MaxAge()) {
        rcvdInfoWhile = 3 * port.PortTimes().HelloTime();
    }

    port.SmTimersInstance().SetRcvdInfoWhile(rcvdInfoWhile);
}

void UpdtRoleDisabledTree(Bridge& bridge) noexcept {
    for (auto& portMapIt : bridge.GetAllPorts()) {
        portMapIt.second->SetSelectedRole(PortRole::Disabled);
    }
}

static RootPathPriority UpdtRolesTreeHelpGetBestPriorityVector(Bridge& bridge) noexcept {
    // The Bridge’s root priority vector (rootPriority plus rootPortId; 17.18.6, 17.18.5), chosen
    // as the best of the set of priority vectors
    RootPathPriority bestRootPriorityVector {
        bridge.RootPortId(), bridge.RootPriority(), bridge.RootTimes()
    };

    for (auto& portMapIt : bridge.GetAllPorts()) {
        Port& port = *(portMapIt.second);
        if (not (Port::Info::Received == port.InfoIs())) {
            continue;
        }

        RootPathPriority rootPathPriority {
            port.PortId(), port.PortPriority(), port.PortTimes()
        };
        // a & 17.6
        rootPathPriority.priorityVector.GetRootPathCost() += port.PortPathCost();

        // All the calculated root path priority vectors whose DesignatedBridgeID Bridge Address
        // component is not equal to that component of the Bridge’s own bridge priority vector
        if (rootPathPriority.priorityVector.DesignatedBridgeId().Address()
                == bridge.BridgePriority().DesignatedBridgeId().Address()) {
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

    return bestRootPriorityVector;
}

static void UpdtRolesTreeHelpUpdatePortRoleAndPortPriority(Bridge& bridge) noexcept {
    for (auto& portMapIt : bridge.GetAllPorts()) {
        Port& port = *(portMapIt.second);
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
                    || (not (port.PortTimes() == bridge.RootTimes()))) {
                port.SetUpdtInfo(true);
            }

            break;
        }
        case Port::Info::Received: {
            if (bridge.RootPortId().PortNum() == port.PortId().PortNum()) {
                // i)
                port.SetSelectedRole(PortRole::Root);
                port.SetUpdtInfo(false);
            }
            else if (not (port.PortPriority() < port.DesignatedPriority())) {
                bool reflected = false;

                /// @todo Confirm that comparing components designatedBridgeId and designatedPortId
                /// are fine without comparing their associated priorities
                for (auto& portMapIt : bridge.GetAllPorts()) {
                    Port& otherPort = *(portMapIt.second);
                    if (otherPort.PortId().PortNum() == port.PortId().PortNum()) {
                        continue;
                    }
                    else if (not (otherPort.PortPriority().DesignatedBridgeId()
                                  == port.PortPriority().DesignatedBridgeId())) {
                        continue;
                    }
                    else if (not (otherPort.PortPriority().DesignatedPortId()
                                  == port.PortPriority().DesignatedPortId())) {
                        continue;
                    }

                    reflected = true;
                    break;
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

void UpdtRolesTree(Bridge& bridge) noexcept {
    // The Bridge’s root priority vector (rootPriority plus rootPortId; 17.18.6, 17.18.5), chosen
    // as the best of the set of priority vectors
    RootPathPriority bestRootPriorityVector {
        UpdtRolesTreeHelpGetBestPriorityVector(bridge)
    };

    if (bestRootPriorityVector.priorityVector == bridge.BridgePriority()) {
        // c1) the chosen root priority vector is the bridge priority vector
        bridge.SetRootTimes(bridge.BridgeTimes());
    }
    else {
        // c2)
        /// @todo Confirm that this is propoer place to update rootPriority and rootPortId components
        bridge.SetRootPriority(bestRootPriorityVector.priorityVector);
        bridge.SetRootPortId(bestRootPriorityVector.portId);

        bridge.SetRootTimes(bestRootPriorityVector.times);
        bridge.GetRootTimes().SetMessageAge(bridge.RootTimes().MessageAge() + 1);
    }

    for (auto& portMapIt : bridge.GetAllPorts()) {
        Port& port = *(portMapIt.second);
        // d)
        port.SetDesignatedPriority(bridge.RootPriority());
        port.GetDesignatedPriority().SetDesignatedBridgeId(bridge.BridgeIdentifier());
        port.GetDesignatedPriority().SetDesignatedPortId(port.PortId());
        port.SetDesignatedTimes(bridge.RootTimes());
        // e)
        port.GetDesignatedTimes().SetHelloTime(bridge.BridgeTimes().HelloTime());
    }

    UpdtRolesTreeHelpUpdatePortRoleAndPortPriority(bridge);
}

} // namespace SmProcedures
} // namespace Stp
