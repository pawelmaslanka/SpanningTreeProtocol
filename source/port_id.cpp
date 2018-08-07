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
#include "priority_vector.hpp"

namespace SpanningTree {

PortId::PortId(const Bpdu::PortIdHandler& portId) noexcept
{
    // Priority is encoded on the most significant nibble (4 bits) of byte
    _priority = ((portId[0] & 0xF0) >> 4) * (u8)PriorityVector::RecommendedPortPriority::Step;
    _portNum = (portId[0] & 0x0F) * (u16)ShiftOctet::CpuLeastSignificant2nd;
    _portNum += portId[1] * (u16)ShiftOctet::CpuLeastSignificant1st;
}

Bpdu::PortIdHandler PortId::ConvertToBpduData() const noexcept
{
    Bpdu::PortIdHandler encodedPortId;
    // Priority is encoded on the most significant nibble (4 bits) of byte
    encodedPortId[0] = (_priority / (u8)PriorityVector::RecommendedPortPriority::Step) << 4;
    // The most significant nibble of Port Number is encoded at the least significant nibble of encoded byte
    encodedPortId[0] |= static_cast<u8>(_portNum / (u16)ShiftOctet::CpuLeastSignificant2nd) & 0x0F;
    // The least significant byte of Port Number is encoded on the least significant byte of Port Identifier
    encodedPortId[1] = static_cast<u8>(_portNum / (u16)ShiftOctet::CpuLeastSignificant1st);

    return encodedPortId;
}

} // namespace Rstp
