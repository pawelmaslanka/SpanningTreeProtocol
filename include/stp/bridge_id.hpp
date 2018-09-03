/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "bpdu.hpp"
#include "lib.hpp"
#include "mac.hpp"

namespace Stp {

class BridgeId {
public:
    /**
     * @brief BridgeId
     * @param encodedData By default, set to value which is considered as the worst assignable value
     * from RSTP point of view.
     */
    BridgeId() noexcept;
    explicit BridgeId(const Bpdu::BridgeIdHandler& bridgeId) noexcept;
    BridgeId(const BridgeId&) noexcept = default;
    BridgeId(BridgeId&&) = default;

    ~BridgeId() noexcept = default;

    BridgeId& operator=(const BridgeId&) = default;
    BridgeId& operator=(BridgeId&&) = default;

    bool operator==(const BridgeId& comparedTo) const noexcept;
    bool operator<(const BridgeId& comparedTo) const noexcept;

    Bpdu::BridgeIdHandler ConvertToBpduData() const noexcept;

    const Mac& Address() const noexcept;
    Mac& GetAddress() noexcept;
    void SetAddress(const Mac& value) noexcept;

    u16 Extension() const noexcept;
    void SetExtension(const u16 value) noexcept;

    u16 Priority() const noexcept;
    void SetPriority(const u16 value) noexcept;

private:
    Mac _addr;
    u16 _ext;
    u16 _priority;
};

inline BridgeId::BridgeId() noexcept : BridgeId({{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }}) { }

inline const Mac& BridgeId::Address() const noexcept { return _addr; }
inline Mac& BridgeId::GetAddress() noexcept { return _addr; }
inline void BridgeId::SetAddress(const Mac& value) noexcept { _addr = value; }

inline u16 BridgeId::Extension() const noexcept { return _ext; }
inline void BridgeId::SetExtension(const u16 value) noexcept { _ext = value; }

inline u16 BridgeId::Priority() const noexcept { return _priority; }
inline void BridgeId::SetPriority(const u16 value) noexcept { _priority = value; }

} // namespace Rstp
