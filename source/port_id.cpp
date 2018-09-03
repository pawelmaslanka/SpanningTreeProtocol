/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// This project's headers
#include "stp/port_id.hpp"
#include "stp/priority_vector.hpp"

namespace Stp {

PortId::PortId(const Bpdu::PortIdHandler& portId) noexcept {
    // Priority is encoded on the most significant nibble (4 bits) of byte
    _priority = ((portId[0] & 0xF0) >> 4) * +PriorityVector::RecommendedPortPriority::Step;
    _portNum = (portId[0] & 0x0F) * +ShiftOctet::CpuLeastSignificant2nd;
    _portNum += portId[1] * +ShiftOctet::CpuLeastSignificant1st;
}

Bpdu::PortIdHandler PortId::ConvertToBpduData() const noexcept {
    Bpdu::PortIdHandler encodedPortId;
    // Priority is encoded on the most significant nibble (4 bits) of byte
    encodedPortId[0] = static_cast<u8>((_priority / +PriorityVector::RecommendedPortPriority::Step) << 4);
    // The most significant nibble of Port Number is encoded at the least significant nibble of encoded byte
    encodedPortId[0] |= static_cast<u8>(_portNum / +ShiftOctet::CpuLeastSignificant2nd) & 0x0F;
    // The least significant byte of Port Number is encoded on the least significant byte of Port Identifier
    encodedPortId[1] = static_cast<u8>(_portNum / +ShiftOctet::CpuLeastSignificant1st);

    return encodedPortId;
}

} // namespace Rstp
