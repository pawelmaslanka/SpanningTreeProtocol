/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "bridge.hpp"
#include "lib.hpp"
#include "perf_params.hpp"
#include "sm_parameters.hpp"

namespace Stp {
namespace SmConditions {

bool AdminEdge(Port& port) noexcept;
bool AutoEdge(Port& port) noexcept;
u16 EdgeDelay(const Port& port) noexcept;
u16 ForwardDelay(const Port& port) noexcept;
enum Port::RcvdInfo RcvInfo(Port& port) noexcept;
bool ReRooted(Bridge& bridge, const Port& port) noexcept;
bool RstpVersion(Bridge& bridge) noexcept;
bool StpVersion(Bridge& bridge) noexcept;

inline bool AdminEdge(Port& port) noexcept {
    return port.adminEdge;
}

inline bool AutoEdge(Port& port) noexcept {
    return port.autoEdge;
}

inline u16 EdgeDelay(const Port& port) noexcept {
    return port.operPointToPointMAC ? PerfParams::MigrateTime() : SmParams::MaxAge(port);
}

inline u16 ForwardDelay(const Port& port) noexcept {
    return port.SendRstp() ? PerfParams::HelloTime(port) : SmParams::FwdDelay(port);
}

inline bool RstpVersion(Bridge& bridge) noexcept {
    return bridge.ForceProtocolVersion >= 2;
}

inline bool StpVersion(Bridge& bridge) noexcept {
    return bridge.ForceProtocolVersion < 2;
}

} // namespace SmConditions
} // namespace Stp
