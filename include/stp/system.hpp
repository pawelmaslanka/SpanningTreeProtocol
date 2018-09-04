/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

#include "logger.hpp"

namespace Stp {

/**
 * @brief The OutInterface class's procedures model the system-dependent actions.
 */
class OutInterface {
public:
    /**
     * @brief FlushFdb removes all entries for the port from the filtering database
     */
    virtual Result FlushFdb(const u16 portNo) __noexcept = 0;
    /**
     * @brief SetForwarding starts or stop forwarding frames through the port
     */
    virtual Result SetForwarding(const u16 portNo, const bool enable) __noexcept = 0;
    /**
     * @brief SetLearning starts or stops learning from frames received on the port
     */
    virtual Result SetLearning(const u16 portNo, const bool enable) __noexcept = 0;
    /**
     * @brief SendOutBpdu transmits frame through the port
     */
    virtual Result SendOutBpdu(const u16 portNo, ByteStreamH data) __noexcept = 0;

protected:
    virtual ~OutInterface() = default;
};

using OutInterfaceH = Sptr<OutInterface>;

struct System {
    System(OutInterfaceH outInterface, LoggingSystem::LoggerH logger);
    OutInterfaceH OutInterface;
    LoggingSystem::LoggerH Logger;
};

using SystemH = Sptr<System>;

inline System::System(OutInterfaceH outInterface, LoggingSystem::LoggerH logger)
    : OutInterface{ outInterface }, Logger{ logger } {
}

} // namespace Stp
