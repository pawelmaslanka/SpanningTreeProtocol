#include "stp/sm/port_protocol_migration.hpp"
//#include "mock.hpp"
#include "stp/management.hpp"
#include "stp/port.hpp"

#include "stp/management.hpp"

#include <iostream>

using namespace Stp;
using namespace std;

class OutInterfaceImpl final : public OutInterface {
public:
    Result FlushFdb(const u16 portNo) { return Result::Success; }
    Result SetForwarding(const u16 portNo, const bool enable) { return Result::Success; }
    Result SetLearning(const u16 portNo, const bool enable) { return Result::Success; }
    Result SendOutBpdu(const u16 portNo, ByteStreamH data) { return Result::Success; }
};

class Logger : public std::ostream {
public:
    Logger() : std::ostream(std::cout.rdbuf()) {}
};

int main() {
    LoggerH logger = std::make_shared<Logger>();
    SystemH system = std::make_shared<System>(Mac{}, logger);
    Stp::Management::RunStp(system, std::make_shared<OutInterfaceImpl>());
    Stp::Management::AddPort(1, 10000, true);

    return 0;
}
