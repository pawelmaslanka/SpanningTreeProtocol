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
    System(OutInterfaceH outInterface, LoggerH logger)
        : OutInterface{ outInterface }, Logger{ logger } {}
    OutInterfaceH OutInterface;
    LoggerH Logger;
};

//class System {
//public:
//    System(Mac bridgeAddr, LoggerH logger);
//    Mac& GetBridgeAddr() noexcept;
//    LoggerH& LogInstance() noexcept;
//private:
//    Mac _bridgeAddr;
//    LoggerH _logger;
//};

using SystemH = Sptr<System>;

} // namespace Stp
