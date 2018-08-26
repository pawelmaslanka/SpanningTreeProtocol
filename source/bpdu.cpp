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
#include "stp/bpdu.hpp"

// C Standard Library
#include <cstring>

// C++ Standard Library
#include <algorithm>
#include <iostream>

namespace Stp {

Bpdu::Bpdu() noexcept
    : _data{ }, _portRole { PortRole::Disabled }, _rootId{ } {
    _data.Fields.BpduType = +Type::Invalid;
}

Bpdu::Bpdu(const Bpdu& copyFrom) noexcept
    : _size{ copyFrom._size } {
    _data.Fields.BpduType = copyFrom._data.Fields.BpduType;
    std::memcpy(reinterpret_cast<void*>(&_data),
                reinterpret_cast<const void*>(&copyFrom._data), sizeof _data);
}

Result Bpdu::Encode(ByteStream& output) noexcept {
    BpduProtocolIdFieldHandler bpduDataProtocolId;
    BpduPathCostFieldHandler bpduDataPathCost;
    BpduTimeFieldHandler bpduDataTime;
    u8 size;

    switch(static_cast<Type>(_data.Fields.BpduType)) {
    case Type::Config:
        size = +Size::Config;
        break;
    case Type::Tcn:
        size = +Size::Tcn;
        break;
    case Type::Rst:
        size = +Size::Rst;
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << "Invalid BPDU type" << std::endl;
        return Result::Fail;
    }

    ConvertEndianessProtocolIdToBpduData(_data.Fields.ProtocolIdentifier,
                                         bpduDataProtocolId);
    std::copy_n(bpduDataProtocolId.begin(), bpduDataProtocolId.size(),
                &_data.encodedStream[+FieldOffset::ProtocolIdentifier]);

    ConvertEndianessPathCostToBpduData(_data.Fields.RootPathCost, bpduDataPathCost);
    std::copy_n(bpduDataPathCost.begin(), bpduDataPathCost.size(),
                &_data.encodedStream[+FieldOffset::RootPathCost]);

    ConvertEndianessTimeToBpduData(_data.Fields.MessageAge, bpduDataTime);
    std::copy_n(bpduDataTime.begin(), bpduDataTime.size(),
                &_data.encodedStream[+FieldOffset::MessageAge]);

    ConvertEndianessTimeToBpduData(_data.Fields.MaxAge, bpduDataTime);
    std::copy_n(bpduDataTime.begin(), bpduDataTime.size(),
                &_data.encodedStream[+FieldOffset::MaxAge]);

    ConvertEndianessTimeToBpduData(_data.Fields.HelloTime, bpduDataTime);
    std::copy_n(bpduDataTime.begin(), bpduDataTime.size(),
                &_data.encodedStream[+FieldOffset::HelloTime]);

    ConvertEndianessTimeToBpduData(_data.Fields.ForwardDelay, bpduDataTime);
    std::copy_n(bpduDataTime.begin(), bpduDataTime.size(),
                &_data.encodedStream[+FieldOffset::ForwardDelay]);

    output.resize(size);
    std::copy_n(&_data.encodedStream[0], size, output.begin());

    if (static_cast<Type>(_data.Fields.BpduType) == Type::Rst) {
        /// @todo Refactor it!
        output.push_back(0x00); output.push_back(0x00); output.push_back(0x00);
    }

    return Result::Success;
}

Result Bpdu::Decode(const ByteStream& input) noexcept {
    ByteStream::size_type streamSize = input.size();

    _data.Fields.ProtocolIdentifier = ConvertEndianessBpduDataToProtocolId({{
                                                                                input[+FieldOffset::ProtocolIdentifier],
                                                                                input[+FieldOffset::ProtocolIdentifier + 1]
                                                                            }});

    if (not IsValidSize(streamSize)
            || not IsValidProtocolId(_data.Fields.ProtocolIdentifier)
            || not IsValidBpduType(input[+FieldOffset::BpduType])) {
        _data.Fields.BpduType = +Type::Invalid;
        return Result::Fail;
    }

    _size = static_cast<enum Size>(streamSize);
    std::copy_n(input.begin(), streamSize, &_data.encodedStream[0]);

    _data.Fields.MessageAge = ConvertEndianessBpduDataToTime({{
                                                                  _data.encodedStream[+FieldOffset::MessageAge],
                                                                  _data.encodedStream[+FieldOffset::MessageAge + 1]
                                                              }});

    _data.Fields.MaxAge = ConvertEndianessBpduDataToTime({{
                                                              _data.encodedStream[+FieldOffset::MaxAge],
                                                              _data.encodedStream[+FieldOffset::MaxAge + 1]
                                                          }});

    // Checking if the received BPDU is not received by port which it transmitted, is outside
    if ((not (Type::Config == _data.Fields.BpduType && Size::Config <= _size
              && _data.Fields.MessageAge < _data.Fields.MaxAge))
            && (not (Type::Tcn == _data.Fields.BpduType && Size::Tcn == _size))
            && (not (Type::Rst == _data.Fields.BpduType && Size::Rst <= _size))) {
        _data.Fields.BpduType = +Type::Invalid;
        return Result::Fail;
    }

    const EncodedPortRole encodedPortRole =
            static_cast<EncodedPortRole>(
                (_data.Fields.Flags >> static_cast<u8>(OffsetFlag::PortRole)) & 0x03
                );

    switch (encodedPortRole) {
    case EncodedPortRole::AlternateBackup:
        _portRole = PortRole::Alternate;
        break;
    case EncodedPortRole::Designated:
        _portRole = PortRole::Designated;
        break;
    case EncodedPortRole::Root:
        _portRole = PortRole::Root;
        break;
    default:
        /// @todo Check if this state is fine for state machine
        _portRole = PortRole::Unknown;
        /// @note If the Unknown value of the Port Role parameter is received, the state
        /// machines will effectively treat the RST BPDU as if it were a Configuration BPDU.
        _data.Fields.BpduType = +Type::Config;
    }

    std::copy_n(&_data.encodedStream[+FieldOffset::RootIdentifier],
            +FieldSize::RootIdentifier, _rootId.begin());

    _data.Fields.RootPathCost =
            ConvertEndianessBpduDataToPathCost({{
                                                    _data.encodedStream[+FieldOffset::RootPathCost],
                                                    _data.encodedStream[+FieldOffset::RootPathCost + 1],
                                                    _data.encodedStream[+FieldOffset::RootPathCost + 2],
                                                    _data.encodedStream[+FieldOffset::RootPathCost + 3]
                                                }});

    std::copy_n(&_data.encodedStream[+FieldOffset::BridgeIdentifier],
            +FieldSize::BridgeIdentifier, _bridgeId.begin());

    _data.Fields.HelloTime =
            ConvertEndianessBpduDataToTime({{
                                                _data.encodedStream[+FieldOffset::HelloTime],
                                                _data.encodedStream[+FieldOffset::HelloTime + 1]
                                            }});

    _data.Fields.ForwardDelay =
            ConvertEndianessBpduDataToTime({{
                                                _data.encodedStream[+FieldOffset::ForwardDelay],
                                                _data.encodedStream[+FieldOffset::ForwardDelay + 1]
                                            }});

    return Result::Success;
}

void Bpdu::SetPortRoleFlag(const PortRole value) noexcept {
    EncodedPortRole portRoleToEncode;

    switch (value) {
    case PortRole::Alternate:
    case PortRole::Backup:
        portRoleToEncode = EncodedPortRole::AlternateBackup;
        break;
    case PortRole::Designated:
        portRoleToEncode = EncodedPortRole::Designated;
        break;
    case PortRole::Root:
        portRoleToEncode = EncodedPortRole::Root;
        break;
    default:
        portRoleToEncode = EncodedPortRole::Unknown;
    }

    _data.Fields.Flags |= (static_cast<u8>(portRoleToEncode) << static_cast<u8>(OffsetFlag::PortRole));
}

void Bpdu::SetRootIdentifier(const Bpdu::BridgeIdHandler& value) noexcept {
    std::copy_n(value.cbegin(), +FieldSize::BridgeIdentifier,
                &_data.encodedStream[+FieldOffset::RootIdentifier]);
}

void Bpdu::SetBridgeIdentifier(const Bpdu::BridgeIdHandler& value) noexcept {
    std::copy_n(value.cbegin(), +FieldSize::BridgeIdentifier,
                &_data.encodedStream[+FieldOffset::BridgeIdentifier]);
}

void Bpdu::SetPortIdentifier(const Bpdu::PortIdHandler& value) noexcept {
    std::copy_n(value.cbegin(), +FieldSize::PortIdentifier,
                &_data.encodedStream[+FieldOffset::PortIdentifier]);
}

u16 Bpdu::ConvertEndianessBpduDataToProtocolId(const Bpdu::BpduProtocolIdFieldHandler& bpduData) noexcept {
    u16 value = bpduData[0];
    value += bpduData[1] * +ShiftOctet::CpuLeastSignificant2nd;

    return value;
}

void Bpdu::ConvertEndianessProtocolIdToBpduData(const u16 protocolId,
                                                Bpdu::BpduProtocolIdFieldHandler& bpduData) noexcept {
    bpduData[0] = static_cast<u8>(protocolId);
    bpduData[1] = static_cast<u8>(protocolId / +ShiftOctet::CpuLeastSignificant2nd);
}

u32 Bpdu::ConvertEndianessBpduDataToPathCost(
        const Bpdu::BpduPathCostFieldHandler& bpduData) noexcept {
    /// @note Path Cost field has different byte order than other fields
    u32 value = bpduData[0] * +ShiftOctet::CpuLeastSignificant4th;
    value += bpduData[1] * +ShiftOctet::CpuLeastSignificant3rd;
    value += bpduData[2] * +ShiftOctet::CpuLeastSignificant2nd;
    value += bpduData[3] * +ShiftOctet::CpuLeastSignificant1st;

    return value;
}

void Bpdu::ConvertEndianessPathCostToBpduData(const u32 pathCost,
                                              Bpdu::BpduPathCostFieldHandler& bpduData) noexcept {
    /// @note Path Cost field has different byte order than other fields
    bpduData[0] = static_cast<u8>(pathCost / +ShiftOctet::CpuLeastSignificant4th);
    bpduData[1] = static_cast<u8>(pathCost / +ShiftOctet::CpuLeastSignificant3rd);
    bpduData[2] = static_cast<u8>(pathCost / +ShiftOctet::CpuLeastSignificant2nd);
    bpduData[3] = static_cast<u8>(pathCost);
}

u16 Bpdu::ConvertEndianessBpduDataToTime(const BpduTimeFieldHandler& bpduData) noexcept {
    u16 value = bpduData[0];
    value += bpduData[1] * +ShiftOctet::CpuLeastSignificant2nd;

    return value;
}

void Bpdu::ConvertEndianessTimeToBpduData(const u16 time, BpduTimeFieldHandler& bpduData) noexcept {
    bpduData[0] = static_cast<u8>(time);
    bpduData[1] = static_cast<u8>(time / +ShiftOctet::CpuLeastSignificant2nd);
}

bool Bpdu::IsValidBpduType(const u8 type) noexcept {
    switch (static_cast<Type>(type)) {
    case Type::Config:
    case Type::Rst:
    case Type::Tcn:
        return true;
    default:
        return false;
    }
}

u8 Bpdu::GetSizeFromBpduType(const Bpdu::Type type) noexcept {
    Size size;

    switch (type) {
    case Type::Config:
        size = Size::Config;
        break;
    case Type::Rst:
        size = Size::Rst;
        break;
    case Type::Tcn:
        size = Size::Tcn;
        break;
    default:
        size = Size::Max;
    }

    return static_cast<u8>(size);
}

} // namespace Rstp
