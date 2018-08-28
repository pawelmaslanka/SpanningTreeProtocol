// This project's headers
#include "stp/logger.hpp"

using namespace Stp;

Logger::Logger(Logger::MsgSeverity msgLogSeverity)
    : _msgSeverityLevel { msgLogSeverity } {
    // Nothing more to do
}

void SystemChangeStateLogger::Log(const char* machineName, const char* oldStateName,
                                  const char* newStateName) {
    // Simple assert for any null pointer
    if (not (machineName && oldStateName && newStateName)) {
        return;
    }

    std::string msg{};
    msg.append(oldStateName).append(" -> ").append(newStateName).append(" @ ").append(machineName);
    *_logger << std::move(msg);
}

void SystemStateEntryLogger::Log(const char* machineName, const char* stateName) {
    // Simple assert for any null pointer
    if (not (machineName && stateName)) {
        return;
    }

    std::string msg{};
    msg.append(stateName).append(" @ ").append(machineName);
    *_logger << std::move(msg);
}
