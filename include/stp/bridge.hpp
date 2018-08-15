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

#ifndef BRIDGE_HPP
#define BRIDGE_HPP

// This project's headers
#include "bridge_id.hpp"
#include "port.hpp"
#include "priority_vector.hpp"
#include "management.hpp"
#include "specifiers.hpp"
#include "time.hpp"

// C++ Standard Library
#include <memory>
#include <vector>

namespace Stp {

class Bridge
{
public:
    // By default, assigned to VLAN #1
    static constexpr uint16_t ExtensionDefaultValue = 1;

    /// @todo Make it dynamic managementable
    static constexpr uint8_t ForceProtocolVersion = 2;

    /// @todo Make it dynamic managementable
    static constexpr u32 AgeingTime = 300;

    std::vector<Port> Ports;

    Bridge() noexcept;
    Bridge(const Bridge&) = default;
    Bridge(Bridge&&) = default;

    ~Bridge() noexcept = default;

    Bridge& operator=(const Bridge&) = default;
    Bridge& operator=(Bridge&&) = default;

    __virtual bool Begin() const __noexcept;
    void SetBegin(const bool value) noexcept;

    const BridgeId& BridgeIdentifier() const noexcept;
    BridgeId& GetBridgeIdentifier() noexcept;
    void SetBridgeIdentifier(const BridgeId& value) noexcept;

    const PriorityVector& BridgePriority() const noexcept;
    PriorityVector& GetBridgePriority() noexcept;
    void SetBridgePriority(const PriorityVector& value) noexcept;

    const Time& BridgeTimes() const noexcept;
    Time& GetBridgeTimes() noexcept;
    void SetBridgeTimes(const Time& value) noexcept;

    const PortId& RootPortId() const noexcept;
    PortId& GetRootPortId() noexcept;
    void SetRootPortId(const PortId& value) noexcept;

    const PriorityVector& RootPriority() const noexcept;
    PriorityVector& GetRootPriority() noexcept;
    void SetRootPriority(const PriorityVector& value) noexcept;

    const Time& RootTimes() const noexcept;
    Time& GetRootTimes() noexcept;
    void SetRootTimes(const Time& value) noexcept;

    const Mac& Address() const noexcept;
    Mac& GetAddress() noexcept;
    void SetAddress(const Mac& value) noexcept;

private:
    /// @brief 17.18.1
    bool _begin;

    /// @brief 17.18.2
    /// @todo Should be dynamic managementable
    BridgeId _bridgeId;

    /// @brief 17.18.3
    /// @todo Should be dynamic managementable
    PriorityVector _bridgePriority;

    /// @brief 17.18.4
    /// @todo Should be dynamic managementable
    Time _bridgeTimes;

    /// @brief 17.18.5
    PortId _rootPortId;

    /// @brief 17.18.6
    PriorityVector _rootPriority;

    /// @brief 17.18.7
    Time _rootTimes;

    Mac _addr;
}; // End of Bridge class declaration

using BridgeH = Bridge&;

inline bool Bridge::Begin() const __noexcept { return _begin; }
inline void Bridge::SetBegin(const bool value) noexcept { _begin = value; }

inline const BridgeId& Bridge::BridgeIdentifier() const noexcept { return _bridgeId; }
inline BridgeId& Bridge::GetBridgeIdentifier() noexcept { return _bridgeId; }
inline void Bridge::SetBridgeIdentifier(const BridgeId& value) noexcept { _bridgeId = value; }

inline const PriorityVector& Bridge::BridgePriority() const noexcept { return _bridgePriority; }
inline PriorityVector& Bridge::GetBridgePriority() noexcept { return _bridgePriority; }
inline void Bridge::SetBridgePriority(const PriorityVector& value) noexcept { _bridgePriority = value; }

inline const Time& Bridge::BridgeTimes() const noexcept { return _bridgeTimes; }
inline Time& Bridge::GetBridgeTimes() noexcept { return _bridgeTimes; }
inline void Bridge::SetBridgeTimes(const Time& value) noexcept { _bridgeTimes = value; }

inline const PortId& Bridge::RootPortId() const noexcept { return _rootPortId; }
inline PortId& Bridge::GetRootPortId() noexcept { return _rootPortId; }
inline void Bridge::SetRootPortId(const PortId& value) noexcept { _rootPortId = value; }

inline const PriorityVector& Bridge::RootPriority() const noexcept { return _rootPriority; }
inline PriorityVector& Bridge::GetRootPriority() noexcept { return _rootPriority; }
inline void Bridge::SetRootPriority(const PriorityVector& value) noexcept { _rootPriority = value; }

inline const Time& Bridge::RootTimes() const noexcept { return _rootTimes; }
inline Time& Bridge::GetRootTimes() noexcept { return _rootTimes; }
inline void Bridge::SetRootTimes(const Time& value) noexcept { _rootTimes = value; }

inline const Mac& Bridge::Address() const noexcept { return _addr; }
inline Mac& Bridge::GetAddress() noexcept { return _addr; }
inline void Bridge::SetAddress(const Mac& value) noexcept { _addr = value; }

} // End of Rstp namespace

#endif // BRIDGE_HPP
