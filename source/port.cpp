/***************************************************************************************************
Copyright (c) 2018, Pawel Maslanka <pawmas@hotmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of the FreeBSD Project.
***************************************************************************************************/

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
