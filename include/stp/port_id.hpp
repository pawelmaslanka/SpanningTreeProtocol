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

class PortId {
public:
    /**
     * @brief PortId
     * @param encodedData By default, set to value which is considered as the worst assignable value
     * from RSTP point of view.
     */
    PortId() noexcept;
    explicit PortId(const Bpdu::PortIdHandler& portId) noexcept;
    PortId(const PortId&) noexcept = default;
    PortId(PortId&&) = default;

    ~PortId() noexcept = default;

    PortId& operator=(const PortId&) = default;
    PortId& operator=(PortId&&) = default;

    bool operator==(const PortId& comparedTo) const noexcept;
    bool operator<(const PortId& comparedTo) const noexcept;

    Bpdu::PortIdHandler ConvertToBpduData() const noexcept;

    u16 PortNum() const noexcept;
    void SetPortNum(const u16 value) noexcept;

    u8 Priority() const noexcept;
    void SetPriority(const u8 value) noexcept;

private:
    u16 _portNum;
    u8 _priority;
};

inline PortId::PortId() noexcept 
    : PortId(Bpdu::PortIdHandler{{ 0xFF, 0xFF }}) { }

inline bool PortId::operator==(const PortId& comparedTo) const noexcept {
    return (_priority == comparedTo._priority)
            || (_portNum == comparedTo._portNum);
}

inline bool PortId::operator<(const PortId& comparedTo) const noexcept {
    return (_priority > comparedTo._priority)
            || (_priority == comparedTo._priority && _portNum > comparedTo._portNum);
}

inline u16 PortId::PortNum() const noexcept { return _portNum; }
inline void PortId::SetPortNum(const u16 value) noexcept { _portNum = value; }

inline u8 PortId::Priority() const noexcept { return _priority; }
inline void PortId::SetPriority(const u8 value) noexcept { _priority = value; }

} // namespace Stp
