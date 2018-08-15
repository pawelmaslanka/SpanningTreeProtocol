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

#ifndef TIMES_HPP
#define TIMES_HPP

// This project's headers
#include "lib.hpp"

namespace Stp {

class Time
{
public:
//    using u16 = uint16_t;

    enum RecommendedValue {
        MigrateTime = 3,
        BridgeHelloTime = 2,
        BridgeMaxAge = 20,
        BridgeForwardDelay = 15
    };

    Time(const u16 msgAge = 0, const u16 maxAge = (u16)RecommendedValue::BridgeMaxAge,
         const u16 fwdDelay = (u16)RecommendedValue::BridgeForwardDelay,
         const u16 helloTime = (u16)RecommendedValue::BridgeHelloTime) noexcept;
    Time(const Time&) noexcept = default;
    Time(Time&&) = default;

    ~Time() noexcept = default;

    Time& operator=(const Time&) noexcept = default;
    Time& operator=(Time&&) = default;

    bool operator==(const Time& comparedTo) const noexcept;

    u16 MessageAge() const noexcept;
    void SetMessageAge(const u16 value) noexcept;

    u16 MaxAge() const noexcept;
    void SetMaxAge(const u16 value) noexcept;

    u16 ForwardDelay() const noexcept;
    void SetForwardDelay(const u16 value) noexcept;

    u16 HelloTime() const noexcept;
    void SetHelloTime(const u16 value) noexcept;

private:
    u16 _msgAge;
    u16 _maxAge;
    u16 _fwdDelay;
    u16 _helloTime;
};

class SmTimers {
public:
    SmTimers() noexcept;
    SmTimers(const SmTimers&) noexcept = default;
    SmTimers(SmTimers&) = default;

    ~SmTimers() noexcept = default;

    SmTimers& operator=(const SmTimers&) noexcept = default;
    SmTimers& operator=(SmTimers&&) = default;

    SmTimers& operator--() noexcept;

    u16 EdgeDelayWhile() const noexcept;
    void SetEdgeDelayWhile(const u16 value) noexcept;

    u16 FdWhile() const noexcept;
    void SetFdWhile(const u16 value) noexcept;

    u16 HelloWhen() const noexcept;
    void SetHelloWhen(const u16 value) noexcept;

    u16 MdelayWhile() const noexcept;
    void SetMdelayWhile(const u16 value) noexcept;

    u16 RbWhile() const noexcept;
    void SetRbWhile(const u16 value) noexcept;

    u16 RcvdInfoWhile() const noexcept;
    void SetRcvdInfoWhile(const u16 value) noexcept;

    u16 RrWhile() const noexcept;
    void SetRrWhile(const u16 value) noexcept;

    u16 TcWhile() const noexcept;
    void SetTcWhile(const u16 value) noexcept;

    static bool TimedOut(const u16 value) noexcept;

private:
    /// @brief 17.17.1
    u16 _edgeDelayWhile;

    /// @brief 17.17.2
    u16 _fdWhile;

    /// @brief 17.17.3
    u16 _helloWhen;

    /// @brief 17.17.4
    u16 _mdelayWhile;

    /// @brief 17.17.5
    u16 _rbWhile;

    /// @brief 17.17.6
    u16 _rcvdInfoWhile;

    /// @brief 17.17.7
    u16 _rrWhile;

    /// @brief 17.17.8
    u16 _tcWhile;
};

inline Time::Time(const u16 msgAge, const u16 maxAge,
                  const u16 fwdDelay, const u16 helloTime) noexcept
    : _msgAge{ msgAge }, _maxAge{ maxAge }, _fwdDelay{ fwdDelay }, _helloTime{ helloTime }
{
    // Nothing more to do
}

inline bool Time::operator==(const Time& comparedTo) const noexcept
{
    return _msgAge == comparedTo._msgAge
            && _maxAge == comparedTo._maxAge
            && _fwdDelay == comparedTo._fwdDelay
            && _helloTime == comparedTo._helloTime;
}

inline u16 Time::MessageAge() const noexcept { return _msgAge; }
inline void Time::SetMessageAge(const u16 value) noexcept { _msgAge = value; }

inline u16 Time::MaxAge() const noexcept { return _maxAge; }
inline void Time::SetMaxAge(const u16 value) noexcept { _maxAge = value; }

inline u16 Time::ForwardDelay() const noexcept { return _fwdDelay; }
inline void Time::SetForwardDelay(const u16 value) noexcept { _fwdDelay = value; }

inline u16 Time::HelloTime() const noexcept { return _helloTime; }
inline void Time::SetHelloTime(const u16 value) noexcept { _helloTime = value; }

inline u16 SmTimers::EdgeDelayWhile() const noexcept { return _edgeDelayWhile; }
inline void SmTimers::SetEdgeDelayWhile(const u16 value) noexcept { _edgeDelayWhile = value; }

inline u16 SmTimers::FdWhile() const noexcept { return _fdWhile; }
inline void SmTimers::SetFdWhile(const u16 value) noexcept { _fdWhile = value; }

inline u16 SmTimers::HelloWhen() const noexcept { return _helloWhen; }
inline void SmTimers::SetHelloWhen(const u16 value) noexcept { _helloWhen = value; }

inline u16 SmTimers::MdelayWhile() const noexcept { return _mdelayWhile; }
inline void SmTimers::SetMdelayWhile(const u16 value) noexcept { _mdelayWhile = value; }

inline u16 SmTimers::RbWhile() const noexcept { return _rbWhile; }
inline void SmTimers::SetRbWhile(const u16 value) noexcept { _rbWhile = value; }

inline u16 SmTimers::RcvdInfoWhile() const noexcept { return _rcvdInfoWhile; }
inline void SmTimers::SetRcvdInfoWhile(const u16 value) noexcept { _rcvdInfoWhile = value; }

inline u16 SmTimers::RrWhile() const noexcept { return _rrWhile; }
inline void SmTimers::SetRrWhile(const u16 value) noexcept { _rrWhile = value; }

inline u16 SmTimers::TcWhile() const noexcept { return _tcWhile; }
inline void SmTimers::SetTcWhile(const u16 value) noexcept { _tcWhile = value; }

inline bool SmTimers::TimedOut(const u16 value) noexcept { return 0 == value; }

} // namespace Rstp

#endif // TIMES_HPP
