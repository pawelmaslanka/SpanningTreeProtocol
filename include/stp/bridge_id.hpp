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
