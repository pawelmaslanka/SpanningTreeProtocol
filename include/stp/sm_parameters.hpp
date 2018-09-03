/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include "port.hpp"

namespace Stp {
namespace SmParams {

u16 FwdDelay(const Port& port) noexcept;
u16 MaxAge(const Port& port) noexcept;

inline u16 FwdDelay(const Port& port) noexcept {
    return port.DesignatedTimes().ForwardDelay();
}

inline u16 MaxAge(const Port& port) noexcept {
    return port.DesignatedTimes().HelloTime();
}

} // namespace SmParams
} // namespace Stp
