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

#ifndef LIB_HPP
#define LIB_HPP

// C Standard Library
#include <climits>
#include <cstdint>

// C++ Standard Library
#include <limits>
#include <memory>
#include <type_traits>
#include <vector>

//class Bridge;
//using BridgeH = std::shared_ptr<Bridge>;
//class Port;
//using PortH = std::shared_ptr<Port>;
//class Machine;
//using MachineH = Machine&;
//class State;
//using StateH = std::shared_ptr<State>;

template <typename T>
using sptr = std::shared_ptr<T>;

namespace Lib {

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

constexpr u8 ByteBitWidth = CHAR_BIT;

enum class BitWidth : u8 {
    u8 = CHAR_BIT,
    u16 = 2 * u8,
    u32 = 2 * u16,
    u64 = 4 * u16
};

inline u8 operator*(const u8 left, const BitWidth right) noexcept
{
    return left * static_cast<u8>(right);
}

using ByteStream = std::vector<u8>;

enum class Boolean : u8 {
    False = 0,
    True = static_cast<u8>(std::numeric_limits<u8>::max())
}; // End of 'Boolean' enumeration declaration

enum class Result : u8 {
    Fail = 0,
    Success = static_cast<u8>(std::numeric_limits<u8>::max())
}; // End of 'Result' enumeration declaration

constexpr inline bool Failed(Result result)
{
    return static_cast<u8>(Result::Fail) == static_cast<u8>(result);
}

template<typename T>
inline void dec(T& timer) noexcept
{
    if (timer) {
        --timer;
    }
}

enum class ShiftOctet : u64 {
    CpuLeastSignificant1st = static_cast<u64>(0x1),
    CpuLeastSignificant2nd = static_cast<u64>(0x100),
    CpuLeastSignificant3rd = static_cast<u64>(0x10000),
    CpuLeastSignificant4th = static_cast<u64>(0x1000000),
    CpuLeastSignificant5th = static_cast<u64>(0x100000000),
    CpuLeastSignificant6th = static_cast<u64>(0x10000000000),
    CpuLeastSignificant7th = static_cast<u64>(0x1000000000000),
    CpuLeastSignificant8th = static_cast<u64>(0x100000000000000)
};

} // End of 'Lib' namespace declaration

namespace SpanningTree {

using namespace Lib;

}

#endif // LIB_HPP
