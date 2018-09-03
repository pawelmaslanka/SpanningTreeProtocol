/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "specifiers.hpp"

// C Standard Library
#include <climits>
#include <cstdint>

// C++ Standard Library
#include <limits>
#include <memory>
#include <type_traits>
#include <vector>

namespace Stp {
namespace Lib {

template <typename T>
using Uptr = std::unique_ptr<T>;
template <typename T>
using Sptr = std::shared_ptr<T>;

using s8 = int8_t;
using u8 = uint8_t;
using s16 = int16_t;
using u16 = uint16_t;
using s32 = int32_t;
using u32 = uint32_t;
using s64 = int64_t;
using u64 = uint64_t;

constexpr u8 ByteBitWidth = CHAR_BIT;

enum class BitWidth : u8 {
    u8 = CHAR_BIT,
    u16 = 2 * u8,
    u32 = 2 * u16,
    u64 = 4 * u16
};

inline u8 operator*(const u8 left, const BitWidth right) noexcept {
    return left * static_cast<u8>(right);
}

using ByteStream = std::vector<u8>;
using ByteStreamH = Sptr<ByteStream>;

enum class Boolean : u8 {
    False = 0,
    True = static_cast<u8>(std::numeric_limits<u8>::max())
}; // End of 'Boolean' enumeration declaration

enum class Result : u8 {
    Fail = 0,
    Success = static_cast<u8>(std::numeric_limits<u8>::max())
}; // End of 'Result' enumeration declaration

constexpr inline bool Failed(Result result) {
    return static_cast<u8>(Result::Fail) == static_cast<u8>(result);
}

template<typename T>
inline void dec(T& timer) noexcept {
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

/// @brief Performs conversion from enum classes to their underlying type
/// @param e Enum class's value
/// @return Value casted onto enum's underlying type
template <typename T>
constexpr auto operator+(T e) noexcept
    -> std::enable_if_t<std::is_enum<T>::value, std::underlying_type_t<T>> {
        return static_cast<std::underlying_type_t<T>>(e);
}

} // namespace Lib

using namespace Stp::Lib;

/// @brief 17.7
enum class PortRole : u8 {
    Unknown,
    Root,
    Designated,
    Alternate,
    Backup,
    Disabled
};
} // namespace Stp
