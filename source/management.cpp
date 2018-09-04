/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

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
              std::make_unique<PortTimers::PtiMachine>(bridge, port),
              std::make_unique<PortReceive::PrxMachine>(bridge, port),
              std::make_unique<PortProtocolMigration::PpmMachine>(bridge, port),
              std::make_unique<BridgeDetection::BdmMachine>(bridge, port),
              std::make_unique<PortTransmit::PtxMachine>(bridge, port),
              std::make_unique<PortInformation::PimMachine>(bridge, port),
              std::make_unique<PortRoleSelection::PrsMachine>(bridge, port),
              std::make_unique<PortRoleTransitions::PrtMachine>(bridge, port),
              std::make_unique<PortStateTransition::PstMachine>(bridge, port),
              std::make_unique<TopologyChange::TcmMachine>(bridge, port)
          } {
        // Nothing more to do
    }

    void TickEvent() {
        static u8 idx;
        for (idx = 0; idx < _kMaxMachines; ++idx) {
            _machines[idx]->Run();
        }
    }

private:
    static constexpr u8 _kMaxMachines = 10;
    MachineH _machines[_kMaxMachines];
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
    void ProcessBpduHandle(ProcessBpduReq& req);
    void SetLogSeverity(SetLogSeverityReq& req);
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
            StpManager::ProcessBpduHandle(dynamic_cast<ProcessBpduReq&>(*req));
            break;
        case RequestId::SetLogSeverity:
            StpManager::SetLogSeverity(dynamic_cast<SetLogSeverityReq&>(*req));
            break;
        default:
            break; // Unhandled user request
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

void StpManager::ProcessBpduHandle(ProcessBpduReq& req) {
    PortH port = _bridge->GetPort(req.GetRxPortNo());
    if (not port) {
        // Received BPDU data from not register port in STP process
        return;
    }

    Bpdu bpdu{};
    if (Failed(bpdu.Decode(req.GetBpduData()))) {
        return;
    }

    if (Bpdu::Type::Config == bpdu.BpduType()) {
        if ((PortId{ bpdu.PortIdentifier() }.PortNum() == req.GetRxPortNo())
                                        &&
            (BridgeId{ bpdu.BridgeIdentifier() }.Address() == _bridge->Address())) {
            // BPDU has been received by port which originally transmitted it...
            // so it's invalid BPDU
            return;
        }
    }

    port->SetRxBpdu(bpdu);
    port->SetRcvdBpdu(true);
}

inline void StpManager::SetLogSeverity(SetLogSeverityReq& req) {
    _bridge->SetSystemLogSeverity(req.GetLogSeverity());
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

Result Management::ProcessBpdu(ByteStreamH bpdu) {
    StpManager::Instance().SubmitRequest(std::make_unique<ProcessBpduReq>(ProcessBpduReq{ bpdu }));
    return Result::Success;
}

Result Management::SetLogSeverity(const LoggingSystem::Logger::LogSeverity logSeverity) {
    StpManager::Instance().SubmitRequest(
                std::make_unique<SetLogSeverityReq>(SetLogSeverityReq{ logSeverity }));
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

} // namespace Stp
