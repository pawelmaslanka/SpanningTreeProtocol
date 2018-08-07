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

#ifndef STATE_MACHINE_HPP
#define STATE_MACHINE_HPP

// This project's headers
#include "bridge.hpp"
#include "port.hpp"
#include "states.hpp"

// C Standard Library
#include <cstdint>

// C++ Standard Library
#include <memory>
#include <unordered_map>

namespace SpanningTree {

class SpanningTreeProtocol
{
public:
    SpanningTreeProtocol(Bridge& bridge, Management& interface) noexcept;

    /// @note Should be called by interval timer
    void TickEvent() noexcept;

private:
    /// @brief 17.22
    void PortTimers(Port &port) noexcept;

    void PortTimersAction(Port& port) noexcept;

    /// @brief 17.23
    void PortReceive(Port& port) noexcept;

    void PortReceiveAction(Port& port) noexcept;

    /// @brief 17.24
    void PortProtocolMigration(Port& port) noexcept;

    void PortProtocolMigrationAction(Port& port) noexcept;

    /// @brief 17.25
    void BridgeDetection(Port& port) noexcept;

    void BridgeDetectionAction(Port& port) noexcept;

    /// @brief 17.26
    void PortTransmit(Port& port) noexcept;

    void PortTransmitAction(Port& port) noexcept;

    /// @brief 17.27
    void PortInformation(Port& port) noexcept;

    void PortInformationAction(Port& port) noexcept;

    /// @brief 17.28
    void PortRoleSelection(Port& port) noexcept;

    void PortRoleSelectionAction(Port& port) noexcept;

    /// @brief 17.29
    void PortRoleTransitions(Port& port) noexcept;

    /// @brief 17.29.1
    void PortRoleTransitionsDisabledState(Port& port) noexcept;

    void PortRoleTransitionsDisabledStateAction(Port& port) noexcept;

    /// @brief 17.29.2
    void PortRoleTransitionsRootState(Port& port) noexcept;

    void PortRoleTransitionsRootStateAction(Port& port) noexcept;

    /// @brief 17.29.3
    void PortRoleTransitionsDesignatedState(Port& port) noexcept;

    void PortRoleTransitionsDesignatedStateAction(Port& port) noexcept;

    /// @brief 17.29.4
    void PortRoleTransitionsAlternateAndBackupState(Port& port) noexcept;

    void PortRoleTransitionsAlternateAndBackupStateAction(Port& port) noexcept;

    /// @brief 17.30
    void PortStateTransition(Port& port) noexcept;

    void PortStateTransitionAction(Port& port) noexcept;

    /// @brief 17.31
    void TopologyChange(Port& port) noexcept;

    void TopologyChangeAction(Port& port) noexcept;

    /// @brief 17.20.1
    bool AdminEdge(const Port& port) const noexcept;

    /// @brief 17.20.2
    bool AutoEdge(const Port &port) const noexcept;

    /// @brief 17.20.3
    bool AllSynced() noexcept;

    /// @brief 17.20.4
    uint16_t EdgeDelay(const Port& port) const noexcept;

    /// @brief 17.20.5
    uint16_t ForwardDelay(const Port &port) const noexcept;

    /// @brief 17.20.6
    uint16_t FwdDelay(const Port& port) const noexcept;

    /// @brief 17.20.7
    uint16_t HelloTime(const Port& port) const noexcept;

    /// @brief 17.20.8
    Time::u16 MaxAge(const Port& port) const noexcept;

    /// @brief 17.20.9
    Time::u16 MigrateTime() const noexcept;

    /// @brief 17.20.10
    bool ReRooted(const Port& port) const noexcept;

    /// @brief 17.20.11
    bool RstpVersion() const noexcept;

    /// @brief 17.20.12
    bool StpVersion() const noexcept;

    /// @brief 17.20.13
    uint8_t TxHoldCount() const noexcept;

    /// @brief 17.21.1
    bool BetterOrSameInfo(const Port& port, const Port::Info newInfoIs) const noexcept;

    /// @brief 17.21.2
    void ClearReselectTree() noexcept;

    /// @brief 17.21.3
    void DisableForwarding(const Port& port) const noexcept;

    /// @brief 17.21.4
    void DisableLearning(const Port& port) const noexcept;

    /// @brief 17.21.5
    void EnableForwarding(const Port& port) const noexcept;

    /// @brief 17.21.6
    void EnableLearning(const Port& port) const noexcept;

    /// @brief 17.21.7
    void NewTcWhile(Port& port) noexcept;

    /// @brief 17.21.8
    enum Port::RcvdInfo RcvInfo(Port& port) noexcept;

    /// @brief 17.21.9
    void RecordAgreement(Port& port) noexcept;

    /// @brief 17.21.10
    void RecordDispute(Port& port) noexcept;

    /// @brief 17.21.11
    void RecordProposal(Port& port) noexcept;

    /// @brief 17.21.12
    void RecordPriority(Port& port) noexcept;

    /// @brief 17.21.13
    void RecordTimes(Port& port) noexcept;

    /// @brief 17.21.14
    void SetSyncTree() noexcept;

    /// @brief 17.21.15
    void SetReRootTree() noexcept;

    /// @brief 17.21.16
    void SetSelectedTree() noexcept;

    /// @brief 17.21.17
    void SetTcFlags(Port& port) noexcept;

    /// @brief 17.21.18
    void SetTcPropTree(const Port& port) const noexcept;

    /// @brief 17.21.19
    void TxConfig(const Port& port) const;

    /// @brief 17.21.20
    void TxRstp(const Port& port) const;

    /// @brief 17.21.21
    void TxTcn(const Port& port) const;

    /// @brief 17.21.22
    void UpdtBpduVersion(Port& port) noexcept;

    /// @brief 17.21.23
    void UpdtRcvdInfoWhile(Port& port) noexcept;

    /// @brief 17.21.24
    void UpdtRoleDisabledTree() noexcept;

    /// @brief 17.21.25
    void UpdtRolesTree() noexcept;

    //    std::unordered_map<uint16_t, std::unique_ptr<Port>> _ports;
    Bridge _bridge;
    std::vector<Port>& _ports;
    Management& _interface;
};

inline bool SpanningTreeProtocol::AdminEdge(const Port &port) const noexcept
{
    return port.adminEdge;
}

inline bool SpanningTreeProtocol::AutoEdge(const Port& port) const noexcept
{
    return port.autoEdge;
}

inline uint16_t SpanningTreeProtocol::EdgeDelay(const Port& port) const noexcept
{
    return port.operPointToPointMAC ? MigrateTime() : MaxAge(port);
}

inline uint16_t SpanningTreeProtocol::ForwardDelay(const Port& port) const noexcept
{
    return port.SendRstp() ? HelloTime(port) : FwdDelay(port);
}

inline u16 SpanningTreeProtocol::MigrateTime() const noexcept
{
    /// @todo Make it dynamic managementable
    return (u16)Time::RecommendedValue::MigrateTime;
}

inline uint16_t SpanningTreeProtocol::FwdDelay(const Port& port) const noexcept
{
    return port.DesignatedTimes().ForwardDelay();
}

inline uint16_t SpanningTreeProtocol::HelloTime(const Port& port) const noexcept
{
    return port.DesignatedTimes().HelloTime();
}

inline bool SpanningTreeProtocol::RstpVersion() const noexcept
{
    return _bridge.ForceProtocolVersion >= 2;
}

inline bool SpanningTreeProtocol::StpVersion() const noexcept
{
    return _bridge.ForceProtocolVersion < 2;
}

inline void SpanningTreeProtocol::ClearReselectTree() noexcept
{
    for (Port& portMap : _ports) {
        portMap.SetReselect(false);
    }
}

inline Time::u16 SpanningTreeProtocol::MaxAge(const Port& port) const noexcept
{
    return port.DesignatedTimes().MaxAge();
}

inline uint8_t SpanningTreeProtocol::TxHoldCount() const noexcept
{
    /// @todo Make it dynamic managementable
    return (uint8_t)Port::RecommendedValue::TransmitHoldCount;
}

} // End of Rstp namespace

#endif // STATE_MACHINE_HPP
