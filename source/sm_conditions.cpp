#include "stp/sm_conditions.hpp"

// C++ Standard Library
#include <iostream>

namespace Stp {
namespace SmConditions {

enum Port::RcvdInfo RcvInfo(Port& port) noexcept {
    if (+Bpdu::Type::Invalid != port.RxBpdu().BpduType()) {
        // Decodes the message priority and timer values from the received BPDU storing them in the
        // msgPriority and msgTimes variables.
        port.GetMsgPriority().SetRootBridgeId(BridgeId(port.RxBpdu().RootIdentifier()));
        port.GetMsgPriority().GetRootPathCost().SetPathCost(port.RxBpdu().RootPathCost());
        port.GetMsgPriority().SetDesignatedBridgeId(BridgeId(port.RxBpdu().BridgeIdentifier()));
        port.GetMsgPriority().SetDesignatedPortId(PortId(port.RxBpdu().PortIdentifier()));

        port.GetMsgTimes().SetMessageAge(port.RxBpdu().MessageAge());
        port.GetMsgTimes().SetMaxAge(port.RxBpdu().MaxAge());
        port.GetMsgTimes().SetHelloTime(port.RxBpdu().HelloTime());
        port.GetMsgTimes().SetForwardDelay(port.RxBpdu().ForwardDelay());
    }

    PortRole encodedPortRole = port.RxBpdu().PortRoleFlag();
    enum Port::RcvdInfo result = Port::RcvdInfo::OtherInfo;

    /// @note A Configuration BPDU explicitly conveys a Designated Port Role.
    if (encodedPortRole != PortRole::Designated) {
        if (Bpdu::Type::Config == port.RxBpdu().BpduType()) {
            if (port.RcvdBpdu()) {
                encodedPortRole = PortRole::Designated;
            }
        }
    }

    switch (encodedPortRole) {
    case PortRole::Designated: {
        if (port.PortPriority() < port.MsgPriority()) {
            /// @note 17.21.8 a1)
            result = Port::RcvdInfo::SuperiorDesignatedInfo;
            break;
        }

        if (port.MsgPriority() == port.PortPriority()) {
            if (port.MsgTimes().ForwardDelay() != port.PortTimes().ForwardDelay()) {
                result = Port::RcvdInfo::SuperiorDesignatedInfo; ///< 17.21.8 a2)
            }
            else if (port.MsgTimes().HelloTime() != port.PortTimes().HelloTime()) {
                result = Port::RcvdInfo::SuperiorDesignatedInfo; ///< 17.21.8 a2)
            }
            else if (port.MsgTimes().MaxAge() != port.PortTimes().MaxAge()) {
                result = Port::RcvdInfo::SuperiorDesignatedInfo; ///< 17.21.8 a2)
            }
            else if (port.MsgTimes().MessageAge() != port.PortTimes().MessageAge()) {
                result = Port::RcvdInfo::SuperiorDesignatedInfo; ///< 17.21.8 a2)
            }
            /// @todo Check if correct is apply this condition
            // According to clause 13.26.6 b2) of 802.1Q-2005
            else /* if (port.InfoIs() == Port::Info::Received) */ {
                /// @note 17.21.8 b)
                result = Port::RcvdInfo::RepeatedDesignatedInfo;
            }

            break;
        }

        /// @note 17.21.8 c)
        result = Port::RcvdInfo::InferiorDesignatedInfo;
        break;
    } // PortRole::Designated
    case PortRole::Root:
    case PortRole::Alternate:
    case PortRole::Backup:
        /// @note 17.21.8 d)
        result = Port::RcvdInfo::InferiorRootAlternateInfo;
        break;
    default:
        std::cerr << __PRETTY_FUNCTION__ << "Invalid encoded role port\n";
    }

    return result;
}

bool ReRooted(Bridge& bridge, const Port& port) noexcept {
    for (const auto& otherPortMapIt : bridge.GetAllPorts()) {
        if (otherPortMapIt.second->PortId().PortNum() == port.PortId().PortNum()) {
            continue;
        }
        else if (SmTimers::TimedOut(port.GetSmTimersInstance().RrWhile())) {
            continue;
        }

        return false;
    }

    return true;
}

} // namespace SmConditions
} // namespace Stp
