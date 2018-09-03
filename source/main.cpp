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
    const u8 bpdu[] = {
        0x00, 0x00, // Protocol Identifier
        0x02, // Protocol Version Identifier
        0x02, // BPDU Type
        0x0f, // BPDU Flags
        // Root Identifier: 32768 / 100 / 00:1c:0e:87:78:00
        0x80, 0x64,// Root Bridge Priority & Root Bridge System ID Extension
        0x00, 0x1c, 0x0e, 0x87, 0x78, 0x00, // Root Bridge System ID
        0x00, 0x00, 0x00, 0x04, // Root Path Cost: 4
        // Bridge Identifier: 32768 / 100 / 00:1c:0e:87:85:00
        0x80, 0x64, // Bridge Priority & Bridge System ID Extension
        0x00, 0x1c, 0x0e, 0x87, 0x85, 0x00, // Bridge System ID
        0x80, 0x04, // Port Identifier: 0x8004
        0x01, 0x00, // Message Age: 1
        0x14, 0x00, // Max Age: 20
        0x02, 0x00, // Hello Time: 2
        0x0f, 0x00, // Forward Delay: 15
        0x00 // Version 1 Length
    };
    LoggingSystem::LoggerH logger = std::make_shared<SystemLogger>(LoggingSystem::Logger::LogSeverity::None);
    SystemH system = std::make_shared<System>(std::make_shared<SystemOutInterface>(), logger);
    Stp::Management::RunStp(Mac{}, system);
    Stp::Management::AddPort(1, 10000 /* MB */, true);
    ByteStreamH bpduData =
            std::make_shared<ByteStream>(&bpdu[0], bpdu + (sizeof (bpdu) / sizeof (bpdu[0])));
    Stp::Management::ProcessBpdu(bpduData);

    return 0;
}
