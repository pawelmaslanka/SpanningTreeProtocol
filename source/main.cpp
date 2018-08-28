#include "stp/sm/port_protocol_migration.hpp"
#include "stp/management.hpp"
#include "stp/port.hpp"

#include "stp/management.hpp"

#include <iostream>

using namespace Stp;
using namespace std;

class SystemOutInterface final : public OutInterface {
public:
    Result FlushFdb(const u16 portNo) noexcept { return Result::Success; }
    Result SetForwarding(const u16 portNo, const bool enable) noexcept { return Result::Success; }
    Result SetLearning(const u16 portNo, const bool enable) noexcept { return Result::Success; }
    Result SendOutBpdu(const u16 portNo, ByteStreamH data) noexcept { return Result::Success; }
};

class SystemLogger : public Logger {
public:
    SystemLogger(Logger::MsgSeverity msgLogSeverity) : Logger{ msgLogSeverity } {}
    void operator<<(std::string&& msg) noexcept override { std::ignore = msg; }
};

int main() {
    LoggerH logger = std::make_shared<SystemLogger>(Logger::MsgSeverity::None);
    SystemH system = std::make_shared<System>(std::make_shared<SystemOutInterface>(), logger);
    Stp::Management::RunStp(Mac{}, system);
    Stp::Management::AddPort(1, 10000, true);

    return 0;
}
