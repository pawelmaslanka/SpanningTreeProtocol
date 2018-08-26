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
#include "stp/path_cost.hpp"

namespace Stp {

u32 PathCost::SpeedMbToPathCostValue(const u32 speedMb) noexcept {
    u32 pathCost;

    // Specification says that recommended value equal to 200000000 should be assign to link speed
    // not greater than 100 Kb/s, but in this case we assume that any speed lesser than 1 Mb/s will
    // share the same value with 100 Kb/s link speed.
    if (speedMb < 1) {
        pathCost = 200000000;
    }
    else if (1 <= speedMb && speedMb < 10) {
        pathCost = 20000000;
    }
    else if (10 <= speedMb && speedMb < 100) {
        pathCost = 2000000;
    }
    else if (100 <= speedMb && speedMb < 1000) {
        pathCost = 200000;
    }
    else if (1000 <= speedMb && speedMb < 10000) {
        pathCost = 20000;
    }
    else if (10000 <= speedMb && speedMb < 100000) {
        pathCost = 2000;
    }
    else if (100000 <= speedMb && speedMb < 1000000) {
        pathCost = 200;
    }
    else if (1000000 <= speedMb && speedMb < 10000000) {
        pathCost = 20;
    }
    else { // 10000000 <= speedMb
        pathCost = 2;
    }

    return pathCost;
}

PathCost::PathCost(const u32 speedMb) noexcept
    : _value{ SpeedMbToPathCostValue(speedMb) } {
    // Nothing more to do
}

u32 PathCost::ConvertToBpduData() const noexcept {
    BpduData bpdu = {{ 0 }};
    bpdu.Fields.MostSignificant1st = static_cast<u8>(_value / +ShiftOctet::CpuLeastSignificant4th);
    bpdu.Fields.MostSignificant2nd = static_cast<u8>(_value / +ShiftOctet::CpuLeastSignificant3rd);
    bpdu.Fields.MostSignificant3rd = static_cast<u8>(_value / +ShiftOctet::CpuLeastSignificant2nd);
    bpdu.Fields.MostSignificant4th = static_cast<u8>(_value / +ShiftOctet::CpuLeastSignificant1st);

    return reinterpret_cast<u32*>(&bpdu.stream[0])[0];
}

} // namespace Rstp
