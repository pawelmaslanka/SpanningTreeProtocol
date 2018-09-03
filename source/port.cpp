/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// This project's headers
#include "stp/bridge.hpp"
#include "stp/port.hpp"

namespace Stp {

Port::Port() noexcept
    : _ageingTime{ Bridge::AgeingTime }, _agree{ false }, _agreed{ false }, _dsgPriority{ },
      _dsgTimes{ }, _disputed{ false }, _fdbFlush{ false }, _forward{ false },
      _forwarding{ false }, _infoIs{ Info::Disabled }, _learn{ false }, _learning{ false },
      _mcheck{ false }, _msgPriority{ }, _msgTimes{ }, _newInfo{ false }, _operEdge{ false },
      _portEnabled{ false }, _portId{ }, _portPathCost{ }, _portPriority{ }, _portTimes{ },
      _proposed{ false }, _proposing{ false }, _rcvdBpdu{ false }, _rcvdInfo{ RcvdInfo::OtherInfo },
      _rcvdMsg{ false }, _rcvdRstp{ false }, _rcvdStp{ false }, _rcvdTc{ false },
      _rcvdTcAck{ false }, _rcvdTcn{ false }, _reRoot{ false }, _reselect{ false },
      _role{ PortRole::Disabled }, _selected{ false }, _selectedRole{ PortRole::Disabled },
      _sendRstp{ false }, _sync{ false }, _synced{ false }, _tcAck{ false }, _tcProp{ false },
      _tick{ false }, _txCount{ +RecommendedValue::TransmitHoldCount }, _updtInfo{ false },
      _rxBpdu{ }, _smTimers{  } {
    _smTimers.SetEdgeDelayWhile(+Time::RecommendedValue::MigrateTime);
    _smTimers.SetFdWhile(+Time::RecommendedValue::BridgeForwardDelay);
    _smTimers.SetHelloWhen(+Time::RecommendedValue::BridgeHelloTime);
}

} // namespace Rstp
