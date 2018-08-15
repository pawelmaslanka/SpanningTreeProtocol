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

#ifndef PORT_HPP
#define PORT_HPP

// This project's headers
#include "bpdu.hpp"
#include "lib.hpp"
#include "port_id.hpp"
#include "priority_vector.hpp"
#include "states.hpp"
#include "time.hpp"

// C++ Standard Library
#include <memory>

namespace Stp {

/**
 * @brief The 'Port' class declares per-port variables based on subclause 17.19 of IEEE Std 802.1D-2004.
 */
class Port {
public:
    /// @todo Make it dynamic managementable
    enum RecommendedValue {
        TransmitHoldCount = 6
    };

    /// @brief 17.19.10 infoIs
    enum class Info : u8 {
        Received,
        Mine,
        Aged,
        Disabled
    };

    /// @brief 17.21.8
    enum class RcvdInfo : u8 {
        SuperiorDesignatedInfo,
        RepeatedDesignatedInfo,
        InferiorDesignatedInfo,
        InferiorRootInfo,
        InferiorRootAlternateInfo,
        OtherInfo
    };

    /// @brief 14.8.2.1.3 j)
    /// @todo Make it dynamic managementable
    static constexpr bool adminEdge = false;

    /// @brief 14.8.2.1.3 l)
    /// @todo Make it dynamic managementable
    static constexpr bool autoEdge = true;

    /// @brief 6.4.3
    /// @todo Make it dynamic managementable
    static constexpr bool operPointToPointMAC = false;

    Port() noexcept;
    Port(const Port&) noexcept = default;
    Port(Port&&) = default;

    ~Port() noexcept = default;

    Port& operator=(const Port&) noexcept = default;
    Port& operator=(Port&&) = default;

    u32 AgeingTime() const noexcept;
    void SetAgeingTime(const u32 value) noexcept;
    void DecAgeingTime() noexcept;

    bool Agree() const noexcept;
    void SetAgree(const bool value) noexcept;

    bool Agreed() const noexcept;
    void SetAgreed(const bool value) noexcept;

    const PriorityVector& DesignatedPriority() const noexcept;
    PriorityVector& GetDesignatedPriority() noexcept;
    void SetDesignatedPriority(const PriorityVector& value) noexcept;

    const Time& DesignatedTimes() const noexcept;
    Time& GetDesignatedTimes() noexcept;
    void SetDesignatedTimes(const Time& value) noexcept;

    bool Disputed() const noexcept;
    void SetDisputed(const bool value) noexcept;

    bool FdbFlush() const noexcept;
    void SetFdbFlush(const bool value) noexcept;

    bool Forward() const noexcept;
    void SetForward(const bool value) noexcept;

    bool Forwarding() const noexcept;
    void SetForwarding(const bool value) noexcept;

    Info InfoIs() const noexcept;
    void SetInfoIs(const Info value) noexcept;

    bool Learn() const noexcept;
    void SetLearn(const bool value) noexcept;

    bool Learning() const noexcept;
    void SetLearning(const bool value) noexcept;

    bool Mcheck() const noexcept;
    void SetMcheck(const bool value) noexcept;

    const PriorityVector& MsgPriority() const noexcept;
    PriorityVector& GetMsgPriority() noexcept;
    void SetMsgPriority(const PriorityVector& value) noexcept;

    const Time& MsgTimes() const noexcept;
    Time& GetMsgTimes() noexcept;
    void SetMsgTimes(const Time&& value) noexcept;

    bool NewInfo() const noexcept;
    void SetNewInfo(const bool value) noexcept;

    bool OperEdge() const noexcept;
    void SetOperEdge(const bool value) noexcept;

    bool PortEnabled() const noexcept;
    void SetPortEnabled(const bool value) noexcept;

    const class PortId& PortId() const noexcept;
    class PortId& GetPortId() noexcept;
    void SetPortId(const class PortId& value) noexcept;

    const PathCost& PortPathCost() const noexcept;
    PathCost& GetPortPathCost() noexcept;
    void SetPortPathCost(const PathCost& value) noexcept;

    const PriorityVector& PortPriority() const noexcept;
    void SetPortPriority(const PriorityVector& value) noexcept;

    const Time& PortTimes() const noexcept;
    Time& GetPortTimes() noexcept;
    void SetPortTimes(const Time& value) noexcept;

    bool Proposed() const noexcept;
    void SetProposed(const bool value) noexcept;

    bool Proposing() const noexcept;
    void SetProposing(const bool value) noexcept;

    bool RcvdBpdu() const noexcept;
    void SetRcvdBpdu(const bool value) noexcept;

    enum RcvdInfo RcvdInfo() const noexcept;
    void SetRcvdInfo(const enum RcvdInfo value) noexcept;

    bool RcvdMsg() const noexcept;
    void SetRcvdMsg(const bool value) noexcept;

    bool RcvdRstp() const noexcept;
    void SetRcvdRstp(const bool value) noexcept;

    bool RcvdStp() const noexcept;
    void SetRcvdStp(const bool value) noexcept;

    bool RcvdTc() const noexcept;
    void SetRcvdTc(const bool value) noexcept;

    bool RcvdTcAck() const noexcept;
    void SetRcvdTcAck(const bool value) noexcept;

    bool RcvdTcn() const noexcept;
    void SetRcvdTcn(const bool value) noexcept;

    bool ReRoot() const noexcept;
    void SetReRoot(const bool value) noexcept;

    bool Reselect() const noexcept;
    void SetReselect(const bool value) noexcept;

    PortRole Role() const noexcept;
    void SetRole(const PortRole value) noexcept;

    bool Selected() const noexcept;
    void SetSelected(const bool value) noexcept;

    PortRole SelectedRole() const noexcept;
    void SetSelectedRole(const PortRole value) noexcept;

    bool SendRstp() const noexcept;
    void SetSendRstp(const bool value) noexcept;

    bool Sync() const noexcept;
    void SetSync(const bool value) noexcept;

    bool Synced() const noexcept;
    void SetSynced(const bool value) noexcept;

    bool TcAck() const noexcept;
    void SetTcAck(const bool value) noexcept;

    bool TcProp() const noexcept;
    void SetTcProp(const bool value) noexcept;

    bool Tick() const noexcept;
    void SetTick(const bool value) noexcept;

    u8 TxCount() const noexcept;
    void SetTxCount(const u8 value) noexcept;
    void DecTxCount() noexcept;
    void IncTxCount() noexcept;

    bool UpdtInfo() const noexcept;
    void SetUpdtInfo(const bool value) noexcept;

    const class Bpdu& RxBpdu() const noexcept;
    class Bpdu& GetRxBpdu() noexcept;
    void SetRxBpdu(const class Bpdu& value) noexcept;

    const SmTimers& GetSmTimersInstance() const noexcept;
    SmTimers& SmTimersInstance() noexcept;
    void SetSmTimers(const SmTimers& value) noexcept;

    enum PtiState PtiState() const noexcept;
    void SetPtiState(const enum PtiState value) noexcept;

    enum PrxState PrxState() const noexcept;
    void SetPrxState(const enum PrxState value) noexcept;

    enum PpmState PpmState() const noexcept;
    void SetPpmState(const enum PpmState value) noexcept;

    enum BdmState BdmState() const noexcept;
    void SetBdmState(const enum BdmState value) noexcept;

    enum PtxState PtxState() const noexcept;
    void SetPtxState(const enum PtxState value) noexcept;

    enum PimState PimState() const noexcept;
    void SetPimState(const enum PimState value) noexcept;

    enum PrsState PrsState() const noexcept;
    void SetPrsState(const enum PrsState value) noexcept;

    enum PrtState PrtState() const noexcept;
    void SetPrtState(const enum PrtState value) noexcept;

    enum PstState PstState() const noexcept;
    void SetPstState(const enum PstState value) noexcept;

    enum TcmState TcmState() const noexcept;
    void SetTcmState(const enum TcmState value) noexcept;

private:
    /// @brief 17.19.1
    u16 _ageingTime;

    /// @brief 17.19.2
    bool _agree;

    /// @brief 17.19.3
    bool _agreed;

    /// @brief 17.19.4
    PriorityVector _dsgPriority;

    /// @brief 17.19.5
    Time _dsgTimes;

    /// @brief 17.19.6
    bool _disputed;

    /// @brief 17.19.7
    bool _fdbFlush;

    /// @brief 17.19.8
    bool _forward;

    /// @brief 17.19.9
    bool _forwarding;

    /// @brief 17.19.10
    Info _infoIs;

    /// @brief 17.19.11
    bool _learn;

    /// @brief 17.19.12
    bool _learning;

    /// @brief 17.19.13
    bool _mcheck;

    /// @brief 17.19.14
    PriorityVector _msgPriority;

    /// @brief 17.19.15
    Time _msgTimes;

    /// @brief 17.19.16
    bool _newInfo;

    /// @brief 17.19.17
    bool _operEdge;

    /// @brief 17.19.18
    bool _portEnabled;

    /// @brief 17.19.19
    class PortId _portId;

    /// @brief 17.19.20
    PathCost _portPathCost;

    /// @brief 17.19.21
    PriorityVector _portPriority;

    /// @brief 17.19.22
    Time _portTimes;

    /// @brief 17.19.23
    bool _proposed;

    /// @brief 17.19.24
    bool _proposing;

    /// @brief 17.19.25
    bool _rcvdBpdu;

    /// @brief 17.19.26
    enum RcvdInfo _rcvdInfo;

    /// @brief 17.19.27
    bool _rcvdMsg;

    /// @brief 17.19.28
    bool _rcvdRstp;

    /// @brief 17.19.29
    bool _rcvdStp;

    /// @brief 17.19.30
    bool _rcvdTc;

    /// @brief 17.19.31
    bool _rcvdTcAck;

    /// @brief 17.19.32
    bool _rcvdTcn;

    /// @brief 17.19.33
    bool _reRoot;

    /// @brief 17.19.34
    bool _reselect;

    /// @brief 17.19.35
    PortRole _role;

    /// @brief 17.19.36
    bool _selected;

    /// @brief 17.19.37
    PortRole _selectedRole;

    /// @brief 17.19.38
    bool _sendRstp;

    /// @brief 17.19.39
    bool _sync;

    /// @brief 17.19.40
    bool _synced;

    /// @brief 17.19.41
    bool _tcAck;

    /// @brief 17.19.42
    bool _tcProp;

    /// @brief 17.19.43
    bool _tick;

    /// @brief 17.19.44
    u8 _txCount;

    /// @brief 17.19.45
    bool _updtInfo;

    class Bpdu _rxBpdu;

    /// @brief Timer used by State Machine
    SmTimers _smTimers;

    enum PtiState _ptiState;

    enum PrxState _prxState;

    enum PpmState _ppmState;

    enum BdmState _bdmState;

    enum PtxState _ptxState;

    enum PimState _pimState;

    enum PrsState _prsState;

    enum PrtState _prtState;

    enum PstState _pstState;

    enum TcmState _tcmState;

}; // End of 'Port' class declaration

using PortH = Port&;

inline u32 Port::AgeingTime() const noexcept { return _ageingTime; }
inline void Port::SetAgeingTime(const u32 value) noexcept { _ageingTime = value; }
inline void Port::DecAgeingTime() noexcept { dec(_ageingTime); }

inline bool Port::Agree() const noexcept { return _agree; }
inline void Port::SetAgree(const bool value) noexcept { _agree = value; }

inline bool Port::Agreed() const noexcept { return _agreed; }
inline void Port::SetAgreed(const bool value) noexcept { _agreed = value; }

inline const PriorityVector& Port::DesignatedPriority() const noexcept { return _dsgPriority; }
inline PriorityVector& Port::GetDesignatedPriority() noexcept { return _dsgPriority; }
inline void Port::SetDesignatedPriority(const PriorityVector& value) noexcept { _dsgPriority = value; }

inline const Time& Port::DesignatedTimes() const noexcept { return _dsgTimes; }
inline Time& Port::GetDesignatedTimes() noexcept { return _dsgTimes; }
inline void Port::SetDesignatedTimes(const Time& value) noexcept { _dsgTimes = value; }

inline bool Port::Disputed() const noexcept { return _disputed; }
inline void Port::SetDisputed(const bool value) noexcept { _disputed = value; }

inline bool Port::FdbFlush() const noexcept { return _fdbFlush; }
inline void Port::SetFdbFlush(const bool value) noexcept { _fdbFlush = value; }

inline bool Port::Forward() const noexcept { return _forward; }
inline void Port::SetForward(const bool value) noexcept { _forward = value; }

inline bool Port::Forwarding() const noexcept { return _forwarding; }
inline void Port::SetForwarding(const bool value) noexcept { _forwarding = value; }

inline Port::Info Port::InfoIs() const noexcept { return _infoIs; }
inline void Port::SetInfoIs(const Port::Info value) noexcept { _infoIs = value; }

inline bool Port::Learn() const noexcept { return _learn; }
inline void Port::SetLearn(const bool value) noexcept { _learn = value; }

inline bool Port::Learning() const noexcept { return _learning; }
inline void Port::SetLearning(const bool value) noexcept { _learning = value; }

inline bool Port::Mcheck() const noexcept { return _mcheck; }
inline void Port::SetMcheck(const bool value) noexcept { _mcheck = value; }

inline const PriorityVector& Port::MsgPriority() const noexcept { return _msgPriority; }
inline PriorityVector& Port::GetMsgPriority() noexcept { return _msgPriority; }
inline void Port::SetMsgPriority(const PriorityVector& value) noexcept { _msgPriority = value; }

inline const Time& Port::MsgTimes() const noexcept { return _msgTimes; }
inline Time& Port::GetMsgTimes() noexcept { return _msgTimes; }
inline void Port::SetMsgTimes(const Time&& value) noexcept { _msgTimes = value; }

inline bool Port::NewInfo() const noexcept { return _newInfo; }
inline void Port::SetNewInfo(const bool value) noexcept { _newInfo = value; }

inline bool Port::OperEdge() const noexcept { return _operEdge; }
inline void Port::SetOperEdge(const bool value) noexcept { _operEdge = value; }

inline bool Port::PortEnabled() const noexcept { return _portEnabled; }
inline void Port::SetPortEnabled(const bool value) noexcept { _portEnabled = value; }

inline const class PortId& Port::PortId() const noexcept { return _portId; }
inline class PortId& Port::GetPortId() noexcept { return _portId; }
inline void Port::SetPortId(const class PortId& value) noexcept { _portId = value; }

inline const PathCost& Port::PortPathCost() const noexcept { return _portPathCost; }
inline PathCost& Port::GetPortPathCost() noexcept { return _portPathCost; }
inline void Port::SetPortPathCost(const PathCost& value) noexcept { _portPathCost = value; }

inline const PriorityVector& Port::PortPriority() const noexcept { return _portPriority; }
inline void Port::SetPortPriority(const PriorityVector& value) noexcept { _portPriority = value; }

inline const Time& Port::PortTimes() const noexcept { return _portTimes; }
inline Time& Port::GetPortTimes() noexcept { return _portTimes; }
inline void Port::SetPortTimes(const Time& value) noexcept { _portTimes = value; }

inline bool Port::Proposed() const noexcept { return _proposed; }
inline void Port::SetProposed(const bool value) noexcept { _proposed = value; }

inline bool Port::Proposing() const noexcept { return _proposing; }
inline void Port::SetProposing(const bool value) noexcept { _proposing = value; }

inline bool Port::RcvdBpdu() const noexcept { return _rcvdBpdu; }
inline void Port::SetRcvdBpdu(const bool value) noexcept { _rcvdBpdu = value; }

inline enum Port::RcvdInfo Port::RcvdInfo() const noexcept { return _rcvdInfo; }
inline void Port::SetRcvdInfo(const enum Port::RcvdInfo value) noexcept { _rcvdInfo = value; }

inline bool Port::RcvdMsg() const noexcept { return _rcvdMsg; }
inline void Port::SetRcvdMsg(const bool value) noexcept { _rcvdMsg = value; }

inline bool Port::RcvdRstp() const noexcept { return _rcvdRstp; }
inline void Port::SetRcvdRstp(const bool value) noexcept { _rcvdRstp = value; }

inline bool Port::RcvdStp() const noexcept { return _rcvdStp; }
inline void Port::SetRcvdStp(const bool value) noexcept { _rcvdStp = value; }

inline bool Port::RcvdTc() const noexcept { return _rcvdTc; }
inline void Port::SetRcvdTc(const bool value) noexcept { _rcvdTc = value; }

inline bool Port::RcvdTcAck() const noexcept { return _rcvdTcAck; }
inline void Port::SetRcvdTcAck(const bool value) noexcept { _rcvdTcAck = value; }

inline bool Port::RcvdTcn() const noexcept { return _rcvdTcn; }
inline void Port::SetRcvdTcn(const bool value) noexcept { _rcvdTcn = value; }

inline bool Port::ReRoot() const noexcept { return _reRoot; }
inline void Port::SetReRoot(const bool value) noexcept { _reRoot = value; }

inline bool Port::Reselect() const noexcept { return _reselect; }
inline void Port::SetReselect(const bool value) noexcept { _reselect = value; }

inline PortRole Port::Role() const noexcept { return _role; }
inline void Port::SetRole(const PortRole value) noexcept { _role = value; }

inline bool Port::Selected() const noexcept { return _selected; }
inline void Port::SetSelected(const bool value) noexcept { _selected = value; }

inline PortRole Port::SelectedRole() const noexcept { return _selectedRole; }
inline void Port::SetSelectedRole(const PortRole value) noexcept { _selectedRole = value; }

inline bool Port::SendRstp() const noexcept { return _sendRstp; }
inline void Port::SetSendRstp(const bool value) noexcept { _sendRstp = value; }

inline bool Port::Sync() const noexcept { return _sync; }
inline void Port::SetSync(const bool value) noexcept { _sync = value; }

inline bool Port::Synced() const noexcept { return _synced; }
inline void Port::SetSynced(const bool value) noexcept { _synced = value; }

inline bool Port::TcAck() const noexcept { return _tcAck; }
inline void Port::SetTcAck(const bool value) noexcept { _tcAck = value; }

inline bool Port::TcProp() const noexcept { return _tcProp; }
inline void Port::SetTcProp(const bool value) noexcept { _tcProp = value; }

inline bool Port::Tick() const noexcept { return _tick; }
inline void Port::SetTick(const bool value) noexcept { _tick = value; }

inline u8 Port::TxCount() const noexcept { return _txCount; }
inline void Port::SetTxCount(const u8 value) noexcept { _txCount = value; }
inline void Port::DecTxCount() noexcept
{
    if (_txCount) {
        --_txCount;
    }
}
inline void Port::IncTxCount() noexcept
{
    if (not (_txCount == std::numeric_limits<uint8_t>::max())) {
        ++_txCount;
    }
}

inline bool Port::UpdtInfo() const noexcept { return _updtInfo; }
inline void Port::SetUpdtInfo(const bool value) noexcept { _updtInfo = value; }

inline const class Bpdu& Port::RxBpdu() const noexcept { return _rxBpdu; }
inline class Bpdu& Port::GetRxBpdu() noexcept { return _rxBpdu; }
inline void Port::SetRxBpdu(const class Bpdu& value) noexcept { _rxBpdu = value; }

inline SmTimers& Port::SmTimersInstance() noexcept { return _smTimers; }
inline const SmTimers& Port::GetSmTimersInstance() const noexcept { return _smTimers; }
inline void Port::SetSmTimers(const SmTimers& value) noexcept { _smTimers = value; }

inline enum PtiState Port::PtiState() const noexcept { return _ptiState; }
inline void Port::SetPtiState(const enum PtiState value) noexcept { _ptiState = value; }

inline enum PrxState Port::PrxState() const noexcept { return _prxState; }
inline void Port::SetPrxState(const enum PrxState value) noexcept { _prxState = value; }

inline enum PpmState Port::PpmState() const noexcept { return _ppmState; }
inline void Port::SetPpmState(const enum PpmState value) noexcept { _ppmState = value; }

inline enum BdmState Port::BdmState() const noexcept { return _bdmState; }
inline void Port::SetBdmState(const enum BdmState value) noexcept { _bdmState = value; }

inline enum PtxState Port::PtxState() const noexcept { return _ptxState; }
inline void Port::SetPtxState(const enum PtxState value) noexcept { _ptxState = value; }

inline enum PimState Port::PimState() const noexcept { return _pimState; }
inline void Port::SetPimState(const enum PimState value) noexcept { _pimState = value; }

inline enum PrsState Port::PrsState() const noexcept { return _prsState; }
inline void Port::SetPrsState(const enum PrsState value) noexcept { _prsState = value; }

inline enum PrtState Port::PrtState() const noexcept { return _prtState; }
inline void Port::SetPrtState(const enum PrtState value) noexcept { _prtState = value; }

inline enum PstState Port::PstState() const noexcept { return _pstState; }
inline void Port::SetPstState(const enum PstState value) noexcept { _pstState = value; }

inline enum TcmState Port::TcmState() const noexcept { return _tcmState; }
inline void Port::SetTcmState(const enum TcmState value) noexcept { _tcmState = value; }

} // End of 'Rstp' namespace declaration

#endif // PORT_HPP
