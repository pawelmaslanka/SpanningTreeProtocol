/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "bpdu.hpp"
#include "lib.hpp"

namespace Stp {

class Mac {
public:
    enum class Size : u8 {
        Address = 6
    };

    /**
     * @brief Mac
     * @param octet1st Most significant byte of MAC-48
     * @param octet2nd Next to most significant byte of first octet of MAC-48
     * @param octet3rd Next to most significant byte of second octet of MAC-48
     * @param octet4th Next to least significant byte of one before last octet of MAC-48
     * @param octet5th Next to least significant byte of last octet of MAC-48
     * @param octet6th Least significant byte of MAC-48
     */

    /**
     * @brief Mac
     * @param encodedData By default, set to value which is considered as the worst assignable value
     * from RSTP point of view.
     */
    explicit Mac() noexcept;
    explicit Mac(const Bpdu::BridgeSystemIdHandler& bridgeSystemId) noexcept;
    Mac(const Mac&) noexcept = default;
    Mac(Mac&&) = default;

    ~Mac() noexcept = default;

    Mac& operator=(const Mac&) = default;
    Mac& operator=(Mac&&) = default;

    bool operator==(const Mac& comparedTo) const noexcept;
    bool operator<(const Mac& comparedTo) const noexcept;

    Bpdu::BridgeSystemIdHandler ConvertToBpduData() const noexcept;
    u64 ConvertToInteger() const noexcept;

    u8 Octet1st() const noexcept;
    void SetOctet1st(const u8 value) noexcept;
    u8 Octet2nd() const noexcept;
    void SetOctet2nd(const u8 value) noexcept;
    u8 Octet3rd() const noexcept;
    void SetOctet3rd(const u8 value) noexcept;
    u8 Octet4th() const noexcept;
    void SetOctet4th(const u8 value) noexcept;
    u8 Octet5th() const noexcept;
    void SetOctet5th(const u8 value) noexcept;
    u8 Octet6th() const noexcept;
    void SetOctet6th(const u8 value) noexcept;

private:
    u64 _addr;
};

inline Mac::Mac() noexcept : Mac({{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }}) { }

inline bool Mac::operator==(const Mac& comparedTo) const noexcept {
    return _addr == comparedTo._addr;
}

inline bool Mac::operator<(const Mac& comparedTo) const noexcept {
    // Lesser value of Mac Address means more preferred value
    return _addr > comparedTo._addr;
}

inline u64 Mac::ConvertToInteger() const noexcept { return _addr; }

inline u8 Mac::Octet1st() const noexcept {
    // reinterpret_cast<> is not allowed in constant expression
    return static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant6th);
}

inline u8 Mac::Octet2nd() const noexcept {
    // reinterpret_cast<> is not allowed in constant expression
    return static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant5th);
}

inline u8 Mac::Octet3rd() const noexcept {
    // reinterpret_cast<> is not allowed in constant expression
    return static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant4th);
}

inline u8 Mac::Octet4th() const noexcept {
    // reinterpret_cast<> is not allowed in constant expression
    return static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant3rd);
}

inline u8 Mac::Octet5th() const noexcept {
    // reinterpret_cast<> is not allowed in constant expression
    return static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant2nd);
}

inline u8 Mac::Octet6th() const noexcept {
    // reinterpret_cast<> is not allowed in constant expression
    return static_cast<u8>(_addr / +ShiftOctet::CpuLeastSignificant1st);
}

} // namespace Rstp
