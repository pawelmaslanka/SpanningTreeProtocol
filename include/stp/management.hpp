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

#ifndef MANAGEMENT_HPP
#define MANAGEMENT_HPP

// This project's headers
#include "bpdu.hpp"
#include "lib.hpp"
#include "mac.hpp"

// C++ Standard Library
#include <array>
#include <mutex>
#include <queue>

namespace SpanningTree {

class Management
{
public:
    using Handler = std::shared_ptr<Management>;

    struct Action {
        enum class Id : u8 {
            AddNewPort,
            UpdateBridgeMacAddr,
            UpdatePortEnable,
            UpdatePortSpeed,
            ReceivedBpdu,
            None
        };

        Id id;
        uint8_t receivedBpdu[(uint8_t)Bpdu::Size::Max];
        Mac bridgeAddr;
        u32 portSpeed;
        uint16_t portNum;
        bool portEnable;

        Action() noexcept;
        Action(const Action& copyFrom) noexcept;
        Action& operator=(const Action& copyFrom) noexcept;
    };

    explicit Management(const Mac& bridgeAddr) noexcept;

    virtual Result PortForwarding(const uint16_t portNum, const bool on) = 0;
    virtual Result PortLearning(const uint16_t portNum, const bool on) = 0;
    virtual Result PortFdbFlush(const uint16_t portNum) = 0;
    virtual Result PortSendBpdu(const uint16_t portNum, const ByteStream& bpdu) = 0;
    Result AddNewPort(const uint16_t portNum, const u32 speed, const bool enable);
    Result SaveReceivedBpdu(const uint16_t portNum, const std::array<uint8_t, (uint8_t)Bpdu::Size::Max>& bpdu);
    Result GetCommand(Action& action);

    Result UpdateBridgeMacAddr(const Mac& addr);
    Result UpdatePortSpeed(const uint16_t portNum, const u32 speed);
    Result UpdatePortEnable(const uint16_t portNum, const bool enable);

    const Mac& BridgeAddr() const noexcept;

private:
    std::queue<Action> _requests;
    std::mutex _requestsMtx;
    Mac _bridgeAddr;
};

inline const Mac& Management::BridgeAddr() const noexcept { return _bridgeAddr; }

void Run(Management::Handler system);

} // namespace Rstp

#endif // MANAGEMENT_HPP
