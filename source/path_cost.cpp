/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

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
