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
#include "lib.hpp"
#include "states.hpp"

// C++ Standard Library
#include <array>
#include <vector>

namespace Stp {

class Bpdu final {
public:
    enum class ProtocolIdentifier : u8 {
        Config = 0x0000,
        Tcn = 0x0000,
        Rst = 0x0000
    };

    enum class ProtocolVersionIdentifier : u8 {
        Config = 0x00,
        Tcn = 0x00,
        Rst = 0x02
    };

    /// @brief 9.2.9
    enum class EncodedPortRole : u8 {
        Unknown = 0,
        AlternateBackup = 1,
        Root = 2,
        Designated = 3
    };

    /// @brief 9.3.3
    enum class OffsetFlag : u8 {
        Tc = 0,
        Proposal = 1,
        PortRole = 2, // actually, role is coded in two-bit field
        Learnig = 4,
        Forwarding = 5,
        Agreement = 6,
        TcAck = 7
    };

    /// @brief
    enum class Version1Length : u8 {
        Rst = 0x00
    };

    /// @brief 9.3 BPDU formats and parameters
    enum class Type : u8 {
        Config = 0x00,     // 9.3.1 Configuration BPDUs
        Tcn = 0x80,        // 9.3.2 Topology Change Notification BPDUs
        Rst = 0x02,        // 9.3.3 Rapid Spanning Tree BPDUs (RST BPDUs)
        Invalid = 0xFF
    };

    enum class Size : u8 {
        Config = 35,    // 9.3.1 Configuration BPDUs
        Tcn = 4,        // 9.3.2 Topology Change Notification BPDUs
        Rst = 36,       // 9.3.3 Rapid Spanning Tree BPDUs (RST BPDUs)
        Min = Tcn,
        Max = 64
    };

    /// @brief Figure 9-3—RST BPDU parameters and format
    enum class FieldSize : u8 {         // Octet
        ProtocolIdentifier = 2,         // 1-2
        ProtocolVersionIdentifier = 1,  // 3
        BpduType = 1,                   // 4
        Flags = 1,                      // 5
        RootIdentifier = 8,             // 6-13
        RootPathCost = 4,               // 14-17
        BridgeIdentifier = 8,           // 18-25
        PortIdentifier = 2,             // 26-27
        MessageAge = 2,                 // 28-29
        MaxAge = 2,                     // 30-31
        HelloTime = 2,                  // 32-33
        ForwardDelay = 2,               // 34-35
        Version1Length = 1,             // 36
        BridgeSystemId = 6,             // MAC address of both Root and Bridge Identifiers
        TimeGroup = 2                   // Size of all times
    };

    enum class FieldOffset : u8 {        // Octets
        ProtocolIdentifier = 0,          // 1-2
        ProtocolVersionIdentifier = 2,   // 3
        BpduType = 3,                    // 4
        Flags = 4,                       // 5
        RootIdentifier = 5,              // 6-13
        RootPathCost = 13,               // 14-17
        BridgeIdentifier = 17,           // 18-25
        PortIdentifier = 25,             // 26-27
        MessageAge = 27,                 // 28-29
        MaxAge = 29,                     // 30-31
        HelloTime = 31,                  // 32-33
        ForwardDelay = 33,               // 34-35
        Version1Length = 35              // 36
    };

    using BridgeIdHandler = std::array<u8, +FieldSize::BridgeIdentifier>;
    using BridgeSystemIdHandler = std::array<u8, +FieldSize::BridgeSystemId>;
    using PortIdHandler = std::array<u8, +FieldSize::PortIdentifier>;

    Bpdu() noexcept;
    Bpdu(const Bpdu& copyFrom) noexcept;

    ~Bpdu() = default;

    Result Encode(ByteStream& output) noexcept;
    Result Decode(const ByteStream& input) noexcept;

    /// @brief 9.3 BPDU formats and parameters
    union DataUnit {
        u8 encodedStream[+Size::Max];
        struct {                            // Octet
            u16 ProtocolIdentifier;         // 1-2
            u8 ProtocolVersionIdentifier;   // 3
            u8 BpduType;                    // 4
            u8 Flags;                       // 5
            u64 RootIdentifier;             // 6-13
            u32 RootPathCost;               // 14-17
            u64 BridgeIdentifier;           // 18-25
            u16 PortIdentifier;             // 26-27
            u16 MessageAge;                 // 28-29
            u16 MaxAge;                     // 30-31
            u16 HelloTime;                  // 32-33
            u16 ForwardDelay;               // 34-35
            u8 Version1Length;              // 36
        } Fields;
    };

    u16 ProtocolIdentifier() const noexcept;
    void SetProtocolIdentifier(const u16 value) noexcept;
    u8 ProtocolVersionIdentifier() const noexcept;
    void SetProtocolVersionIdentifier(const u8 value) noexcept;
    u8 BpduType() const noexcept;
    void SetBpduType(const u8 value) noexcept;
    u8 TcAckFlag() const noexcept;
    void SetTcAckFlag() noexcept;
    void ClearTcAckFlag() noexcept;
    u8 AgreementFlag() const noexcept;
    void SetAgreementFlag() noexcept;
    void ClearAgreementFlag() noexcept;
    u8 ForwardingFlag() const noexcept;
    void SetForwardingFlag() noexcept;
    void ClearForwardingFlag() noexcept;
    u8 LearnigFlag() const noexcept;
    void SetLearnigFlag() noexcept;
    void ClearLearnigFlag() noexcept;
    PortRole PortRoleFlag() const noexcept;
    void SetPortRoleFlag(const PortRole value) noexcept;
    u8 ProposalFlag() const noexcept;
    void SetProposalFlag() noexcept;
    void ClearProposalFlag() noexcept;
    u8 TcFlag() const noexcept;
    void SetTcFlag() noexcept;
    void ClearTcFlag() noexcept;
    const BridgeIdHandler& RootIdentifier() const noexcept;
    void SetRootIdentifier(const BridgeIdHandler& value) noexcept;
    u32 RootPathCost() const noexcept;
    void SetRootPathCost(const u32 value) noexcept;
    const BridgeIdHandler& BridgeIdentifier() const noexcept;
    void SetBridgeIdentifier(const BridgeIdHandler& value) noexcept;
    const PortIdHandler& PortIdentifier() const noexcept;
    void SetPortIdentifier(const PortIdHandler& value) noexcept;
    u16 MessageAge() const noexcept;
    void SetMessageAge(const u16 value) noexcept;
    u16 MaxAge() const noexcept;
    void SetMaxAge(const u16 value) noexcept;
    u16 HelloTime() const noexcept;
    void SetHelloTime(const u16 value) noexcept;
    u16 ForwardDelay() const noexcept;
    void SetForwardDelay(const u16 value) noexcept;
    u8 Version1Length() const noexcept;
    void SetVersion1Length(const u8 value) noexcept;

private:
    using BpduProtocolIdFieldHandler = std::array<u8, +FieldSize::ProtocolIdentifier>;
    using BpduPathCostFieldHandler = std::array<u8, +FieldSize::RootPathCost>;
    using BpduTimeFieldHandler = std::array<u8, +FieldSize::TimeGroup>;

    enum class FlagMask : u8 {
        PortRole = 0x03
    };

    static constexpr bool IsValidProtocolId(const u16 protocolId) noexcept;

    static constexpr inline bool IsValidSize(const ByteStream::size_type streamSize) noexcept;

    static u16 ConvertEndianessBpduDataToProtocolId(const BpduProtocolIdFieldHandler& bpduData) noexcept;
    static void ConvertEndianessProtocolIdToBpduData(const u16 protocolId,
                                                     BpduProtocolIdFieldHandler& bpduData) noexcept;
    static u32 ConvertEndianessBpduDataToPathCost(
            const BpduPathCostFieldHandler& bpduData) noexcept;
    static void ConvertEndianessPathCostToBpduData(const u32 pathCost,
                                                   BpduPathCostFieldHandler& bpduData) noexcept;
    static u16 ConvertEndianessBpduDataToTime(const BpduTimeFieldHandler& bpduData) noexcept;
    static void ConvertEndianessTimeToBpduData(const u16 time,
                                               BpduTimeFieldHandler& bpduData) noexcept;

    bool IsValidBpduType(const u8 type) noexcept;
    u8 GetSizeFromBpduType(const Type type) noexcept;

    DataUnit _data;
    Size _size;
    PortRole _portRole;
    BridgeIdHandler _rootId;
    BridgeIdHandler _bridgeId;
    PortIdHandler _portId;
};

constexpr inline bool Bpdu::IsValidProtocolId(const u16 protocolId) noexcept {
    return 0x0000 == protocolId;
}

constexpr inline bool Bpdu::IsValidSize(const ByteStream::size_type streamSize) noexcept {
    return +Size::Min <= streamSize && streamSize <= +Size::Max;
}

inline u16 Bpdu::ProtocolIdentifier() const noexcept { return _data.Fields.ProtocolIdentifier; }
inline void Bpdu::SetProtocolIdentifier(const u16 value) noexcept { _data.Fields.ProtocolIdentifier = value; }

inline u8 Bpdu::ProtocolVersionIdentifier() const noexcept { return _data.Fields.ProtocolVersionIdentifier; }
inline void Bpdu::SetProtocolVersionIdentifier(const u8 value) noexcept { _data.Fields.ProtocolVersionIdentifier = value; }

inline u8 Bpdu::BpduType() const noexcept { return _data.Fields.BpduType; }
inline void Bpdu::SetBpduType(const u8 value) noexcept { _data.Fields.BpduType = value; }

inline u8 Bpdu::TcAckFlag() const noexcept {
    return (_data.Fields.Flags >> static_cast<u8>(OffsetFlag::TcAck)) & 0x01;
}

inline void Bpdu::SetTcAckFlag() noexcept {
    _data.Fields.Flags |= (1 << static_cast<u8>(OffsetFlag::TcAck));
}

inline void Bpdu::ClearTcAckFlag() noexcept {
    _data.Fields.Flags &= ~(1 << static_cast<u8>(OffsetFlag::TcAck));
}

inline u8 Bpdu::AgreementFlag() const noexcept {
    return (_data.Fields.Flags >> static_cast<u8>(OffsetFlag::Agreement)) & 0x01;
}

inline void Bpdu::SetAgreementFlag() noexcept {
    _data.Fields.Flags |= (1 << static_cast<u8>(OffsetFlag::Agreement));
}

inline void Bpdu::ClearAgreementFlag() noexcept {
    _data.Fields.Flags &= ~(1 << static_cast<u8>(OffsetFlag::Agreement));
}

inline u8 Bpdu::ForwardingFlag() const noexcept {
    return (_data.Fields.Flags >> static_cast<u8>(OffsetFlag::Forwarding)) & 0x01;
}

inline void Bpdu::SetForwardingFlag() noexcept {
    _data.Fields.Flags |= (1 << static_cast<u8>(OffsetFlag::Forwarding));
}

inline void Bpdu::ClearForwardingFlag() noexcept {
    _data.Fields.Flags &= ~(1 << static_cast<u8>(OffsetFlag::Forwarding));
}

inline u8 Bpdu::LearnigFlag() const noexcept {
    return (_data.Fields.Flags >> static_cast<u8>(OffsetFlag::Learnig)) & 0x01;
}

inline void Bpdu::SetLearnigFlag() noexcept {
    _data.Fields.Flags |= (1 << static_cast<u8>(OffsetFlag::Learnig));
}

inline void Bpdu::ClearLearnigFlag() noexcept {
    _data.Fields.Flags &= ~(1 << static_cast<u8>(OffsetFlag::Learnig));
}

inline PortRole Bpdu::PortRoleFlag() const noexcept {
    return _portRole;
}

inline u8 Bpdu::ProposalFlag() const noexcept {
    return (_data.Fields.Flags >> static_cast<u8>(OffsetFlag::Proposal)) & 0x01;
}

inline void Bpdu::SetProposalFlag() noexcept {
    _data.Fields.Flags |= (1 << static_cast<u8>(OffsetFlag::Proposal));
}

inline void Bpdu::ClearProposalFlag() noexcept {
    _data.Fields.Flags &= ~(1 << static_cast<u8>(OffsetFlag::Proposal));
}

inline u8 Bpdu::TcFlag() const noexcept {
    return (_data.Fields.Flags >> static_cast<u8>(OffsetFlag::Tc)) & 0x01;
}

inline void Bpdu::SetTcFlag() noexcept {
    _data.Fields.Flags |= (1 << static_cast<u8>(OffsetFlag::Tc));
}

inline void Bpdu::ClearTcFlag() noexcept {
    _data.Fields.Flags &= ~(1 << static_cast<u8>(OffsetFlag::Tc));
}

inline const Bpdu::BridgeIdHandler& Bpdu::RootIdentifier() const noexcept { return _rootId; }

inline u32 Bpdu::RootPathCost() const noexcept { return _data.Fields.RootPathCost; }
inline void Bpdu::SetRootPathCost(const u32 value) noexcept { _data.Fields.RootPathCost = value; }

inline const Bpdu::BridgeIdHandler& Bpdu::BridgeIdentifier() const noexcept { return _bridgeId; }

inline const Bpdu::PortIdHandler& Bpdu::PortIdentifier() const noexcept { return _portId; }

inline u16 Bpdu::MessageAge() const noexcept { return _data.Fields.MessageAge; }
inline void Bpdu::SetMessageAge(const u16 value) noexcept { _data.Fields.MessageAge = value; }

inline u16 Bpdu::MaxAge() const noexcept { return _data.Fields.MaxAge; }
inline void Bpdu::SetMaxAge(const u16 value) noexcept { _data.Fields.MaxAge = value; }

inline u16 Bpdu::HelloTime() const noexcept { return _data.Fields.HelloTime; }
inline void Bpdu::SetHelloTime(const u16 value) noexcept { _data.Fields.HelloTime = value; }

inline u16 Bpdu::ForwardDelay() const noexcept { return _data.Fields.ForwardDelay; }
inline void Bpdu::SetForwardDelay(const u16 value) noexcept { _data.Fields.ForwardDelay = value; }

inline u8 Bpdu::Version1Length() const noexcept { return _data.Fields.Version1Length; }
inline void Bpdu::SetVersion1Length(const u8 value) noexcept { _data.Fields.Version1Length = value; }

inline bool operator==(const Bpdu::Type left, const u8 right) noexcept {
    return static_cast<u8>(left) == right;
}

inline bool operator==(const Bpdu::EncodedPortRole left, const u8 right) noexcept {
    return static_cast<u8>(left) == right;
}

inline u8 operator>>(const u8 left, const Bpdu::OffsetFlag right) noexcept {
    return left >> static_cast<u8>(right);
}

} // namespace Stp
