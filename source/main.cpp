#include "port_protocol_migration_sm.hpp"
//#include "mock.hpp"
#include "port.hpp"

#include <iostream>

using namespace SpanningTree;
using namespace std;

int main() {
    SpanningTree::Bridge bridge;
    SpanningTree::Port port;
    PortProtocolMigration::Machine machine(bridge, port);
    machine.Run();
    machine.Run();
    machine.Run();

    return 0;
}
//#include <iostream>
//#include <array>
//#include <vector>

//#include "bpdu.hpp"
//#include "lib.hpp"
//#include "management.hpp"

//using namespace std;
//    using namespace Rstp;

//class RstpInterfaceImpl : public Rstp::Management {
//public:
//    RstpInterfaceImpl() : Management(Mac({{ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }})) { }
//    virtual Rstp::Result PortForwarding(const u16 portNum, const bool on) override { return Rstp::Result::Success; }
//    virtual Rstp::Result PortLearning(const u16 portNum, const bool on) override { return Rstp::Result::Success; }
//    virtual Rstp::Result PortFdbFlush(const u16 portNum) override { return Rstp::Result::Success; }
//    virtual Rstp::Result PortSendBpdu(const u16 portNum, const ByteStream& bpdu) override { return Rstp::Result::Success; }
//};

//int main()
//{
//    Management::Handler rstpInterface{ new RstpInterfaceImpl };

//    Run(rstpInterface);

//    Management::Action action;
//    action.id = Management::Action::Id::AddNewPort;

//    if (Failed(rstpInterface->AddNewPort(1, 1000, true))) {
//        std::cout << "Failed to add new port" << std::endl;
//    }

//    return 0;
//}
