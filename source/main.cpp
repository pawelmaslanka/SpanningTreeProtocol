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

class SystemLogger : public LoggingSystem::Logger {
public:
    SystemLogger(LoggingSystem::Logger::LogSeverity msgLogSeverity)
        : LoggingSystem::Logger{ msgLogSeverity } {}
    void operator<<(std::string&& msg) noexcept override { std::ignore = msg; }
};

int main() {
    LoggingSystem::LoggerH logger = std::make_shared<SystemLogger>(LoggingSystem::Logger::LogSeverity::None);
    SystemH system = std::make_shared<System>(std::make_shared<SystemOutInterface>(), logger);
    Stp::Management::RunStp(Mac{}, system);
    Stp::Management::AddPort(1, 10000 /* MB */, true);

    return 0;
}
