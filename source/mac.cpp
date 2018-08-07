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
#include "mac.hpp"

namespace SpanningTree {

Mac::Mac(const Bpdu::BridgeSystemIdHandler& bridgeSystemId) noexcept
{
    _addr = bridgeSystemId[0] * (u64)ShiftOctet::CpuLeastSignificant6th;
    _addr += bridgeSystemId[1] * (u64)ShiftOctet::CpuLeastSignificant5th;
    _addr += bridgeSystemId[2] * (u64)ShiftOctet::CpuLeastSignificant4th;
    _addr += bridgeSystemId[3] * (u64)ShiftOctet::CpuLeastSignificant3rd;
    _addr += bridgeSystemId[4] * (u64)ShiftOctet::CpuLeastSignificant2nd;
    _addr += bridgeSystemId[5] * (u64)ShiftOctet::CpuLeastSignificant1st;
}

Bpdu::BridgeSystemIdHandler Mac::ConvertToBpduData() const noexcept
{
    Bpdu::BridgeSystemIdHandler bridgeSystemId;

    bridgeSystemId[0] = static_cast<u8>(_addr / (u64)ShiftOctet::CpuLeastSignificant6th);
    bridgeSystemId[1] = static_cast<u8>(_addr / (u64)ShiftOctet::CpuLeastSignificant5th);
    bridgeSystemId[2] = static_cast<u8>(_addr / (u64)ShiftOctet::CpuLeastSignificant4th);
    bridgeSystemId[3] = static_cast<u8>(_addr / (u64)ShiftOctet::CpuLeastSignificant3rd);
    bridgeSystemId[4] = static_cast<u8>(_addr / (u64)ShiftOctet::CpuLeastSignificant2nd);
    bridgeSystemId[5] = static_cast<u8>(_addr / (u64)ShiftOctet::CpuLeastSignificant1st);

    return bridgeSystemId;
}

void Mac::SetOctet1st(const u8 value) noexcept
{
    _addr -= Octet1st() * (u64)ShiftOctet::CpuLeastSignificant6th;
    _addr += value * (u64)ShiftOctet::CpuLeastSignificant6th;
}

void Mac::SetOctet2nd(const u8 value) noexcept
{
    _addr -= Octet2nd() * (u64)ShiftOctet::CpuLeastSignificant5th;
    _addr += value * (u64)ShiftOctet::CpuLeastSignificant5th;
}

void Mac::SetOctet3rd(const u8 value) noexcept
{
    _addr -= Octet3rd() * (u64)ShiftOctet::CpuLeastSignificant4th;
    _addr += value * (u64)ShiftOctet::CpuLeastSignificant4th;
}

void Mac::SetOctet4th(const u8 value) noexcept
{
    _addr -= Octet4th() * (u64)ShiftOctet::CpuLeastSignificant3rd;
    _addr += value * (u64)ShiftOctet::CpuLeastSignificant3rd;
}

void Mac::SetOctet5th(const u8 value) noexcept
{
    _addr -= Octet5th() * (u64)ShiftOctet::CpuLeastSignificant2nd;
    _addr += value * (u64)ShiftOctet::CpuLeastSignificant2nd;
}

void Mac::SetOctet6th(const u8 value) noexcept
{
    _addr -= Octet6th() * (u64)ShiftOctet::CpuLeastSignificant1st;
    _addr += value * (u64)ShiftOctet::CpuLeastSignificant1st;
}

} // namespace Rstp
