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
     * @param portNo number of port related with the filtering database
     * @return Result::Success if operation completed with success, otherwise Result::Fail
     */
    virtual Result FlushFdb(const u16 portNo) __noexcept = 0;
    /**
     * @brief SetForwarding starts or stop forwarding frames through the port
     * @param portNo number of port on which requested to set state into forwarding
     * @param enable flag which indicates if port should go into forwarding state
     * @return Result::Success if operation completed with success, otherwise Result::Fail
     */
    virtual Result SetForwarding(const u16 portNo, const bool enable) __noexcept = 0;
    /**
     * @brief SetLearning starts or stops learning from frames received on the port
     * @param portNo number of port on which requested to set state into learning
     * @param enable flag which indicates if port should go into learning state
     * @return Result::Success if operation completed with success, otherwise Result::Fail
     */
    virtual Result SetLearning(const u16 portNo, const bool enable) __noexcept = 0;
    /**
     * @brief SendOutBpdu transmits frame through the port
     * @param portNo number of port on which BPDU should be send
     * @return Result::Success if operation completed with success, otherwise Result::Fail
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
