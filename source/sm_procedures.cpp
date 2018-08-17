// This project's headers
#include "stp/sm_procedures.hpp"
// Dependencies
#include "stp/bpdu.hpp"
#include "stp/sm_conditions.hpp"
#include "stp/time.hpp"

// C++ Standard Library
#include <iostream>

namespace Stp {
namespace SmProcedures {

bool BetterOrSameInfo(PortH port, const Port::Info newInfoIs) noexcept {
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

void RecordAgreement(BridgeH bridge, PortH port) noexcept {
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


void RecordDispute(PortH port) noexcept {
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

void RecordPriority(PortH port) noexcept {
    port.SetPortPriority(port.MsgPriority());
}

void RecordProposal(PortH port) noexcept {
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

void RecordTimes(PortH port) noexcept {
    port.GetPortTimes().SetMessageAge(port.MsgTimes().MessageAge());
    port.GetPortTimes().SetMaxAge(port.MsgTimes().MaxAge());
    port.GetPortTimes().SetForwardDelay(port.MsgTimes().ForwardDelay());
    // 1 is minimum compatability range value of Hello Time parameter
    const u16 helloTime = port.MsgTimes().HelloTime() > 1 ? port.MsgTimes().HelloTime() : 1;
    port.GetPortTimes().SetHelloTime(helloTime);
}

void SetTcFlags(PortH port) noexcept {
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

void TxConfig(PortH port) {
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
        std::runtime_error(std::string{"Not implemented "} + __func__ + ":"
                           + std::to_string(__LINE__));
        //        _interface.PortSendBpdu(port.PortId().PortNum(), bpduStream);
    }
}

void TxRstp(PortH port) {
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

    bpdu.SetVersion1Length((u8)Bpdu::Version1Length::Rst);

    if (Failed(bpdu.Encode(bpduStream))) {
        std::cerr << __PRETTY_FUNCTION__ << "Failed to encode BPDU unit data\n";
    }
    else {
        std::runtime_error(std::string{"Not implemented "} + __func__ + ":"
                           + std::to_string(__LINE__));
        //        _interface.PortSendBpdu(port.PortId().PortNum(), bpduStream);
    }
}

void TxTcn(PortH port) {
    ByteStream bpduStream;
    Bpdu bpdu;

    bpdu.SetProtocolIdentifier(static_cast<u16>(Bpdu::ProtocolIdentifier::Tcn));
    bpdu.SetProtocolVersionIdentifier(static_cast<u8>(Bpdu::ProtocolVersionIdentifier::Tcn));
    bpdu.SetBpduType(static_cast<u8>(Bpdu::Type::Tcn));

    if (Failed(bpdu.Encode(bpduStream))) {
        std::cerr << __PRETTY_FUNCTION__ << "Failed to encode BPDU unit data\n";
    }
    else {
        std::runtime_error(std::string{"Not implemented "} + __func__ + ":"
                           + std::to_string(__LINE__));
        //        _interface.PortSendBpdu(port.PortId().PortNum(), bpduStream);
    }
}

void UpdtBpduVersion(PortH port) noexcept {
    if (Bpdu::Type::Rst == port.RxBpdu().BpduType()) {
        port.SetRcvdStp(false);
        port.SetRcvdRstp(true);
    }
    else {
        port.SetRcvdRstp(false);
        port.SetRcvdStp(true);
    }
}

void UpdtRcvdInfoWhile(PortH port) noexcept {
    u16 rcvdInfoWhile = {};
    if ((port.PortTimes().MessageAge() + 1) <= port.PortTimes().MaxAge()) {
        rcvdInfoWhile = 3 * port.PortTimes().HelloTime();
    }

    port.SmTimersInstance().SetRcvdInfoWhile(rcvdInfoWhile);
}

} // namespace SmProcedures
} // namespace SpanningTree
