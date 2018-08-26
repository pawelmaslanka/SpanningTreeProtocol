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
#include "stp/bridge_id.hpp"
#include "stp/priority_vector.hpp"

// C++ Standard Library
#include <algorithm>

namespace Stp {

BridgeId::BridgeId(const Bpdu::BridgeIdHandler& bridgeId) noexcept
    : _addr({{ bridgeId[2], bridgeId[3], bridgeId[4], bridgeId[5], bridgeId[6], bridgeId[7] }}) {
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
