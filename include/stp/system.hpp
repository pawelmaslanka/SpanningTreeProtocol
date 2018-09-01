#pragma once

#include "logger.hpp"

namespace Stp {

class OutInterface {
public:
    virtual Result FlushFdb(const u16 portNo) __noexcept = 0;
    virtual Result SetForwarding(const u16 portNo, const bool enable) __noexcept = 0;
    virtual Result SetLearning(const u16 portNo, const bool enable) __noexcept = 0;
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
