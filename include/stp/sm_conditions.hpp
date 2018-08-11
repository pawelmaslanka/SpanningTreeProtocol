#pragma once

#include "bridge.hpp"
#include "lib.hpp"

namespace SpanningTree {

class SmConditions {
public:
//    static StateMachineParameters& Instance() noexcept;
    static bool AdminEdge(PortH port) noexcept;
    static bool AutoEdge(PortH port) noexcept;
    static bool RstpVersion(BridgeH bridge) noexcept;
};

//inline bool StateMachineParameters::AdminEdge(const Port &port) const noexcept
//{
//    return port.adminEdge;
//}

//inline bool StateMachineParameters::AutoEdge(const Port& port) const noexcept
//{
//    return port.autoEdge;
//}

//inline uint16_t StateMachineParameters::EdgeDelay(const Port& port) const noexcept
//{
//    return port.operPointToPointMAC ? MigrateTime() : MaxAge(port);
//}

//inline uint16_t StateMachineParameters::ForwardDelay(const Port& port) const noexcept
//{
//    return port.SendRstp() ? HelloTime(port) : FwdDelay(port);
//}

//inline Time::BaseType StateMachineParameters::MigrateTime() const noexcept
//{
//    /// @todo Make it dynamic managementable
//    return (Time::BaseType)Time::RecommendedValue::MigrateTime;
//}

//inline uint16_t StateMachineParameters::FwdDelay(const Port& port) const noexcept
//{
//    return port.DesignatedTimes().ForwardDelay();
//}

//inline uint16_t StateMachineParameters::HelloTime(const Port& port) const noexcept
//{
//    return port.DesignatedTimes().HelloTime();
//}

//inline bool StateMachineParameters::RstpVersion() const noexcept
//{
//    return _bridge.ForceProtocolVersion >= 2;
//}

//inline bool StateMachineParameters::StpVersion() const noexcept
//{
//    return _bridge.ForceProtocolVersion < 2;
//}

//inline void StateMachineParameters::ClearReselectTree() noexcept
//{
//    for (Port& portMap : _ports) {
//        portMap.SetReselect(false);
//    }
//}

//inline Time::BaseType StateMachineParameters::MaxAge(const Port& port) const noexcept
//{
//    return port.DesignatedTimes().MaxAge();
//}

//inline uint8_t StateMachineParameters::TxHoldCount() const noexcept
//{
//    /// @todo Make it dynamic managementable
//    return (uint8_t)Port::RecommendedValue::TransmitHoldCount;
//}

inline bool SmConditions::AdminEdge(PortH port) noexcept {
    return port.adminEdge;
}

inline bool SmConditions::AutoEdge(PortH port) noexcept {
    return port.autoEdge;
}

inline bool SmConditions::RstpVersion(BridgeH bridge) noexcept {
    return bridge.ForceProtocolVersion >= 2;
}

} // namespace SpanningTree
