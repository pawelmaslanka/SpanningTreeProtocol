// This project's headers
#include "stp/management.hpp"
// Dependencies
#include "stp/state_machine.hpp"
#include "stp/sm/port_timers.hpp"
#include "stp/sm/port_receive.hpp"
#include "stp/sm/port_protocol_migration.hpp"
#include "stp/sm/bridge_detection.hpp"
#include "stp/sm/port_transmit.hpp"
#include "stp/sm/port_information.hpp"
#include "stp/sm/port_role_selection.hpp"
#include "stp/sm/port_role_transitions.hpp"
#include "stp/sm/port_state_transition.hpp"
#include "stp/sm/topology_change.hpp"

// C++ Standard Library
#include <chrono>
#include <functional>
#include <future>
#include <map>
#include <queue>
#include <thread>
#include <utility>

using namespace Stp;

class StateMachine {
public:
    StateMachine(BridgeH bridge, PortH port)
        : _machines {
              Machine{ bridge, port, PortTimers::BeginState::Instance() },
              Machine{ bridge, port, PortReceive::BeginState::Instance() },
              Machine{ bridge, port, PortProtocolMigration::BeginState::Instance() },
              Machine{ bridge, port, BridgeDetection::BeginState::Instance() },
              Machine{ bridge, port, PortTransmit::BeginState::Instance() },
              Machine{ bridge, port, PortInformation::BeginState::Instance() },
              Machine{ bridge, port, PortRoleSelection::BeginState::Instance() },
              Machine{ bridge, port, PortRoleTransitions::BeginState::Instance() },
              Machine{ bridge, port, PortStateTransition::BeginState::Instance() },
              Machine{ bridge, port, TopologyChange::BeginState::Instance() }
          } {
        // Nothing more to do
    }

    void TickEvent() {
        static u8 idx;
        for (idx = 0; idx < _kMaxMachines; ++idx) {
            _machines[idx].Run();
        }
    }

private:
    static constexpr u8 _kMaxMachines = 10;
    Machine _machines[_kMaxMachines];
};

class StpManager {
public:
    static StpManager& Instance();
    Result StpBegin(Mac bridgeAddr, SystemH system);
    void SubmitRequest(Uptr<Command> req);

protected:
    StpManager() = default;

private:
    void AddPortHandle(AddPortReq& req);
    void RemovePortHandle(RemovePortReq& req);
    void RunStateMachine();
    void ProcessRequest();
    BridgeH _bridge;
    std::queue<Uptr<Command>> _userRequests;
    std::mutex _mtxUserRequests;
    std::map<u16, StateMachine> _runningStateMachines;
};

StpManager& StpManager::Instance() {
    static StpManager instance{};
    return instance;
}

Result StpManager::StpBegin(Mac bridgeAddr, SystemH system) {
    _bridge = std::make_shared<Bridge>(system);
    _bridge->SetAddress(bridgeAddr);
    _bridge->GetBridgeIdentifier().SetAddress(bridgeAddr);
    _bridge->GetBridgePriority().GetRootPathCost().SetPathCost(0);
    _bridge->GetBridgePriority().SetDesignatedBridgeId(_bridge->BridgeIdentifier());
    _bridge->SetRootPriority(_bridge->BridgePriority());
    _bridge->SetBegin(true);

    using namespace std::chrono_literals;
    using MiliSec = std::chrono::milliseconds;
    std::chrono::milliseconds sleepTime {};
    constexpr MiliSec kSleepTime250ms { 250 };
    constexpr u16 kMaxSleepCounterToRunStateMachine = { 4 }; // 4 * 250ms = 1s interval for run SM
    u16 sleepCounter250ms = {};
    std::chrono::milliseconds startProcessReq {};
    std::chrono::duration<s64, std::milli> elapsedTimeForProcessReq {};
    auto now = []() {
        auto actualTime = std::chrono::high_resolution_clock::now();
        return std::chrono::time_point_cast<std::chrono::milliseconds>(actualTime).time_since_epoch();
    };

    while (true) {
        ++sleepCounter250ms;
        if (kMaxSleepCounterToRunStateMachine == sleepCounter250ms) {
            RunStateMachine();
            sleepCounter250ms = 0;
        }

        startProcessReq = now();
        ProcessRequest();
        elapsedTimeForProcessReq = now() - startProcessReq;
        sleepTime = kSleepTime250ms - elapsedTimeForProcessReq;
        if (sleepTime.count() > 0) {
            std::this_thread::sleep_for(sleepTime);
        }
    }

    return Result::Success;
}

void StpManager::SubmitRequest(Uptr<Command> req) {
    std::lock_guard<std::mutex> requestsGuard{ _mtxUserRequests };
    _userRequests.push(std::move(req));
}

inline void StpManager::RunStateMachine() {
    for (auto& sm : _runningStateMachines) {
        sm.second.TickEvent();
    }
}

void StpManager::ProcessRequest() {
    Uptr<Command> req{}; // Represents single client request to perform
    std::unique_lock<std::mutex> requestsGuard{ _mtxUserRequests };
    requestsGuard.unlock();
    while (true) {
        if (_userRequests.empty()) {
            requestsGuard.unlock();
            return;
        }

        req.reset(_userRequests.front().release());
        _userRequests.pop();
        requestsGuard.unlock();
        switch (req->Id()) {
        case RequestId::AddPort:
            StpManager::AddPortHandle(dynamic_cast<AddPortReq&>(*req));
            break;
        case RequestId::RemovePort:
            StpManager::RemovePortHandle(dynamic_cast<RemovePortReq&>(*req));
            break;
        case RequestId::ProcessBpdu:
            //            StpManager::ProcessBpduHandle(dynamic_cast<ProcessBpduReq&>(*req));
            break;
        default:
            throw std::runtime_error{ "Unexpected user request command" };
            break;
        }

        req.reset();
    }
}

void StpManager::AddPortHandle(AddPortReq& req) {
    if (_bridge->GetPort(req.GetPortNo())) {
        return;
    }

    _bridge->AddPort(req.GetPortNo());
    PortH newPort = _bridge->GetPort(req.GetPortNo());
    newPort->SetPortEnabled(req.GetPortEnabled());
    newPort->GetPortPathCost().SetPathCost(PathCost::SpeedMbToPathCostValue(req.GetPortSpeed()));
    newPort->GetPortId().SetPortNum(req.GetPortNo());
    newPort->GetPortId().SetPriority(+PriorityVector::RecommendedPortPriority::Value);
    _runningStateMachines.insert(std::make_pair(req.GetPortNo(),
                                                StateMachine{ _bridge, newPort }));
}

void StpManager::RemovePortHandle(RemovePortReq& req) {
    _runningStateMachines.erase(req.GetPortNo());
    _bridge->RemovePort(req.GetPortNo());
}

namespace Stp {

Result Management::AddPort(const u16 portNo, const u32 speed, const bool enabled) {
    StpManager::Instance().SubmitRequest(
                std::make_unique<AddPortReq>(AddPortReq{portNo, speed, enabled}));
    return Result::Success;
}

Result Management::RemovePort(const u16 portNo) {
    StpManager::Instance().SubmitRequest(std::make_unique<RemovePortReq>(RemovePortReq{ portNo }));
    return Result::Success;
}

Result Management::RunStp(Mac bridgeAddr, SystemH system) {
    static std::unique_ptr<std::future<Result>> runnableStp;

    if (not runnableStp) {
        runnableStp.reset(new std::future<Result>{
                              std::async(std::launch::async, &StpManager::StpBegin,
                              &StpManager::Instance(), bridgeAddr, system)
                          });
    }
    else {
        return  Result::Fail;
    }

    return Result::Success;
}

AddPortReq::AddPortReq(const u16 portNo, const u32 speed, const bool enabled)
    : Command{ RequestId::AddPort }, _speed{ speed }, _portNo{ portNo }, _enabled{ enabled } {

}

RemovePortReq::RemovePortReq(const u16 portNo)
    : Command{ RequestId::RemovePort }, _portNo{ portNo } {

}

} // namespace Stp
