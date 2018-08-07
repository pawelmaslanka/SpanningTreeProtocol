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
#include "bridge.hpp"
#include "port_id.hpp"
#include "management.hpp"
#include "state_machine.hpp"

// C Standard Library
#include <cstring>

// C++ Standard Library
#include <chrono>
#include <functional>
#include <future>
#include <thread>

#include <iostream>

namespace Rstp {

Management::Management(const Mac &bridgeAddr) noexcept
    : _bridgeAddr{ bridgeAddr }
{

}

Result Management::AddNewPort(const u16 portNum, const u32 speed, const bool enable)
{
    Action action;
    action.portNum = portNum;
    action.portSpeed = speed;
    action.portEnable = enable;
    action.id = Action::Id::AddNewPort;
    std::lock_guard<std::mutex> lck(_requestsMtx);
    _requests.push(std::move(action));

    return Result::Success;
}

Result Management::SaveReceivedBpdu(const u16 portNum,
                                       const std::array<u8, (u8)Bpdu::Size::Max>& bpdu)
{
    Action action;
    std::memcpy(action.receivedBpdu, bpdu.data(), sizeof action.receivedBpdu);
    action.portNum = portNum;
    action.id = Action::Id::ReceivedBpdu;
    std::lock_guard<std::mutex> lck(_requestsMtx);
    _requests.push(std::move(action));

    return Result::Success;
}

Result Management::GetCommand(Management::Action& action)
{
    if (_requests.empty()) {
        return Result::Fail;
    }

    std::lock_guard<std::mutex> lck(_requestsMtx);
    action = _requests.front();
    _requests.pop();

    return Result::Success;
}

Result Management::UpdateBridgeMacAddr(const Mac& addr)
{
    Action action;
    action.bridgeAddr = addr;
    action.id = Action::Id::UpdateBridgeMacAddr;
    _requests.push(std::move(action));
    _bridgeAddr = addr;

    return Result::Success;
}

Result Management::UpdatePortSpeed(const u16 portNum, const u32 speed)
{
    Action action;
    action.portNum = portNum;
    action.portSpeed = speed;
    action.id = Action::Id::UpdatePortSpeed;
    _requests.push(std::move(action));

    return Result::Success;
}

Result Management::UpdatePortEnable(const u16 portNum, const bool enable)
{
    Action action;
    action.portNum = portNum;
    action.portEnable = enable;
    action.id = Action::Id::UpdatePortEnable;
    _requests.push(std::move(action));

    return Result::Success;
}

Management::Action& Management::Action::operator=(const Action& copyFrom) noexcept
{
    std::memcpy(receivedBpdu, copyFrom.receivedBpdu, sizeof receivedBpdu);
    id = copyFrom.id;
    bridgeAddr = copyFrom.bridgeAddr;
    portSpeed = copyFrom.portSpeed;
    portNum = copyFrom.portNum;
    portEnable = copyFrom.portEnable;

    return *this;
}

Management::Action::Action() noexcept
    : id{ Action::Id::None }, receivedBpdu{ 0x00 }, bridgeAddr{ {} }, portSpeed{ 0 }, portNum{ 0 },
      portEnable{ false }
{
    // Nothing more to do
}

Management::Action::Action(const Management::Action& copyFrom) noexcept
    : bridgeAddr{ copyFrom.bridgeAddr }
{
    *this = copyFrom;
}

Result RstpBegin(Management::Handler system)
{
    Bridge bridge;
    bridge.SetAddress(system->BridgeAddr());
    bridge.GetBridgeIdentifier().SetAddress(system->BridgeAddr());
    bridge.GetBridgePriority().GetRootPathCost().SetPathCost(0);
    bridge.GetBridgePriority().SetDesignatedBridgeId(bridge.BridgeIdentifier());
    bridge.SetRootPriority(bridge.BridgePriority());
    bridge.SetBegin(true);

    StateMachine stateMachine(bridge, *system);

    Management::Action action;
    std::vector<Port>::iterator requestedPort;

    static constexpr u8 quarterPerSec = 4;

    for (u8 quarterSecCount = 0; true; ++quarterSecCount) {
        while (not Failed(system->GetCommand(action))) {
            requestedPort = bridge.Ports.end();

            for (std::vector<Port>::iterator port = bridge.Ports.begin();
                 port != bridge.Ports.end(); ++port) {
                if (port->PortId().PortNum() == action.portNum) {
                    requestedPort = port;
                    break;
                }
            }

            if (bridge.Ports.end() == requestedPort
                    && Management::Action::Id::AddNewPort != action.id) {
                std::cerr << __PRETTY_FUNCTION__ << " Invalid request\n";
                break;
            }

            switch(action.id) {
            case Management::Action::Id::AddNewPort: {
                Port port;
                port.SetPortEnabled(action.portEnable);

                port.GetPortPathCost().SetPathCost(PathCost::SpeedMbToPathCostValue(action.portSpeed));
                port.GetPortId().SetPortNum(action.portNum);
                port.GetPortId().SetPriority((u8)PriorityVector::RecommendedPortPriority::Value);

                bridge.Ports.push_back(port);
                break;
            }
            case Management::Action::Id::UpdateBridgeMacAddr: {
                bridge.SetAddress(action.bridgeAddr);
                bridge.GetBridgeIdentifier().SetAddress(action.bridgeAddr);
                bridge.GetBridgePriority().SetDesignatedBridgeId(bridge.BridgeIdentifier());
                bridge.SetRootPriority(bridge.BridgePriority());
                break;
            }
            case Management::Action::Id::UpdatePortSpeed: {
                requestedPort->GetPortPathCost().SetPathCost(PathCost::SpeedMbToPathCostValue(action.portSpeed));
                break;
            }
            case Management::Action::Id::UpdatePortEnable: {
                requestedPort->SetPortEnabled(action.portEnable);
                break;
            }
            case Management::Action::Id::ReceivedBpdu: {
                Bpdu bpdu;
                ByteStream bpduStream(&action.receivedBpdu[0],
                        &action.receivedBpdu[0] + sizeof (action.receivedBpdu));

                if (Failed(bpdu.Decode(bpduStream))) {
                    break;
                }

                if (Bpdu::Type::Config == bpdu.BpduType()) {
                    if ((PortId(bpdu.PortIdentifier()).PortNum() == action.portNum)
                            && (BridgeId(bpdu.BridgeIdentifier()).Address() == bridge.Address())) {
                        // BPDU has been received by port which originally transmitted it...
                        // so it's invalid BPDU
                        break;
                    }
                }

                requestedPort->SetRxBpdu(bpdu);
                requestedPort->SetRcvdBpdu(true);

                break;
            }
            default:
                std::cerr << __PRETTY_FUNCTION__ << " Invalid ID of request\n";
                break;
            }
        }

        if (quarterPerSec == quarterSecCount) {
            stateMachine.TickEvent();
            quarterSecCount = 0;
        }

        // sleep thread for quarter of second (250 ms)
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    return Result::Fail;
}

void Run(Management::Handler system)
{
    static std::unique_ptr<std::future<Result>> taskHandler;

    if (not taskHandler) {
        taskHandler.reset(new std::future<Result>{ std::async(std::launch::async, RstpBegin, system) });
    }
}

} // namespace Rstp
