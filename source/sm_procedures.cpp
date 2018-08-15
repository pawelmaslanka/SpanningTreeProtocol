// This project's headers
#include "stp/sm_procedures.hpp"
// Dependencies
#include "stp/bpdu.hpp"
#include "stp/time.hpp"

// C++ Standard Library
#include <iostream>

namespace Stp {
namespace SmProcedures {

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

} // namespace SmProcedures
} // namespace SpanningTree
