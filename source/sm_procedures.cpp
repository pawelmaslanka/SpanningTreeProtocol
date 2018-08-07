// This project's headers
#include "sm_procedures.hpp"

#include "bpdu.hpp"

namespace SpanningTree {

void SmProcedures::UpdtBpduVersion(PortH port) noexcept {
    if (Bpdu::Type::Rst == port.RxBpdu().BpduType()) {
        port.SetRcvdStp(false);
        port.SetRcvdRstp(true);
    }
    else {
        port.SetRcvdRstp(false);
        port.SetRcvdStp(true);
    }
}

}
