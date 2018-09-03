/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#include "stp/mac.hpp"

namespace Stp {

Mac::Mac(const Bpdu::BridgeSystemIdHandler& bridgeSystemId) noexcept {
    _addr = bridgeSystemId[0] * +ShiftOctet::CpuLeastSignificant6th;
    _addr += bridgeSystemId[1] * +ShiftOctet::CpuLeastSignificant5th;
    _addr += bridgeSystemId[2] * +ShiftOctet::CpuLeastSignificant4th;
    _addr += bridgeSystemId[3] * +ShiftOctet::CpuLeastSignificant3rd;
    _addr += bridgeSystemId[4] * +ShiftOctet::CpuLeastSignificant2nd;
    _addr += bridgeSystemId[5] * +ShiftOctet::CpuLeastSignificant1st;
}

Bpdu::BridgeSystemIdHandler Mac::ConvertToBpduData() const noexcept {
    Bpdu::BridgeSystemIdHandler bridgeSystemId;

    bridgeSystemId[0] = static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant6th);
    bridgeSystemId[1] = static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant5th);
    bridgeSystemId[2] = static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant4th);
    bridgeSystemId[3] = static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant3rd);
    bridgeSystemId[4] = static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant2nd);
    bridgeSystemId[5] = static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant1st);

    return bridgeSystemId;
}

void Mac::SetOctet1st(const u8 value) noexcept {
    _addr -= Octet1st() * +ShiftOctet::CpuLeastSignificant6th;
    _addr += value * +ShiftOctet::CpuLeastSignificant6th;
}

void Mac::SetOctet2nd(const u8 value) noexcept {
    _addr -= Octet2nd() * +ShiftOctet::CpuLeastSignificant5th;
    _addr += value * +ShiftOctet::CpuLeastSignificant5th;
}

void Mac::SetOctet3rd(const u8 value) noexcept {
    _addr -= Octet3rd() * +ShiftOctet::CpuLeastSignificant4th;
    _addr += value * +ShiftOctet::CpuLeastSignificant4th;
}

void Mac::SetOctet4th(const u8 value) noexcept {
    _addr -= Octet4th() * +ShiftOctet::CpuLeastSignificant3rd;
    _addr += value * +ShiftOctet::CpuLeastSignificant3rd;
}

void Mac::SetOctet5th(const u8 value) noexcept {
    _addr -= Octet5th() * +ShiftOctet::CpuLeastSignificant2nd;
    _addr += value * +ShiftOctet::CpuLeastSignificant2nd;
}

void Mac::SetOctet6th(const u8 value) noexcept {
    _addr -= Octet6th() * +ShiftOctet::CpuLeastSignificant1st;
    _addr += value * +ShiftOctet::CpuLeastSignificant1st;
}

} // namespace Rstp
