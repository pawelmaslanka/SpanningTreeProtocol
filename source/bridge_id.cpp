/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// This project's headers
#include "stp/bridge_id.hpp"
#include "stp/priority_vector.hpp"

// C++ Standard Library
#include <algorithm>

namespace Stp {

// bridgeId[0] & bridgeId[1] makes priority number
BridgeId::BridgeId(const Bpdu::BridgeIdHandler& bridgeId) noexcept
    : _addr(Bpdu::BridgeSystemIdHandler{{ bridgeId[2], bridgeId[3],
                                          bridgeId[4], bridgeId[5], 
                                          bridgeId[6], bridgeId[7] 
                                       }}) {
    // Priority is encoded on more significant nibble of byte
    _priority = ((bridgeId[0] & 0xF0) >> 4) * +PriorityVector::RecommendedBridgePriority::Step;
    // The most significant nibble of system extension is encoded on the least significant nibble
    _ext = (bridgeId[0] & 0x0F) * +ShiftOctet::CpuLeastSignificant2nd;
    _ext += bridgeId[1] * +ShiftOctet::CpuLeastSignificant1st;
}

bool BridgeId::operator==(const BridgeId& comparedTo) const noexcept {
    return (_priority == comparedTo._priority) && (_addr == comparedTo._addr);
}

bool BridgeId::operator<(const BridgeId& comparedTo) const noexcept {
    // Greater value of priority means less preferred value
    return (_priority > comparedTo._priority)
            || ((_priority == comparedTo._priority) && (_addr < comparedTo._addr));
}

Bpdu::BridgeIdHandler BridgeId::ConvertToBpduData() const noexcept {
    Bpdu::BridgeIdHandler bridgeId;

    // Priority is encoded on more significant nibble of byte
    bridgeId[0] = static_cast<u8>((_priority / +PriorityVector::RecommendedBridgePriority::Step) << 4);
    // The most significant byte of extension is encoded on lesser significant nibble of byte
    bridgeId[0] += static_cast<u8>((_ext / +ShiftOctet::CpuLeastSignificant2nd) & 0x0F);
    bridgeId[1] = static_cast<u8>(_ext / +ShiftOctet::CpuLeastSignificant1st);

    std::copy_n(_addr.ConvertToBpduData().cbegin(),
                +Bpdu::FieldSize::BridgeSystemId,
                bridgeId.begin() + 2);

    return bridgeId;
}

} // namespace Rstp
