// This project's headers
#include "stp/sm_procedures.hpp"

#include "stp/bpdu.hpp"

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
