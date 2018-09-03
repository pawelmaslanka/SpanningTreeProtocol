/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// This project's headers
#include "stp/logger.hpp"

namespace Stp {
namespace LoggingSystem {

Logger::Logger(Logger::LogSeverity logSeverity)
    : _logSeverity { logSeverity } {
    // Nothing more to do
}

void SystemChangeState::Log(const std::string& machineName, const std::string& oldStateName,
                            const std::string& newStateName) {
    std::string msg{};
    msg.append(oldStateName).append(" -> ").append(newStateName).append(" @ ").append(machineName);
    _logger << std::move(msg);
}

void SystemEntryState::Log(const std::string& machineName, const std::string& stateName) {
    std::string msg{};
    msg.append(stateName).append(" @ ").append(machineName);
    _logger << std::move(msg);
}

SystemLoggingManager::SystemLoggingManager(LoggerH logger)
    : _logger{ logger }, _nullEntryStateLogger{ *logger }, _systemEntryStateLogger { *logger },
      _nullChangeStateLogger{ *logger }, _systemChangeStateLogger { *logger } {
    _entryStateLogger = &_nullEntryStateLogger;
    _changeStateLogger = &_nullChangeStateLogger;
}

void SystemLoggingManager::SetLogSeverity(const Logger::LogSeverity logSeverity) {
    if (logSeverity == Logger::LogSeverity::None) {
        _entryStateLogger = &_nullEntryStateLogger;
        _changeStateLogger = &_nullChangeStateLogger;
    }
    else if (logSeverity == Logger::LogSeverity::EntryState) {
        _entryStateLogger = &_systemEntryStateLogger;
    }
    else if (logSeverity == Logger::LogSeverity::ChangeState) {
        _changeStateLogger = &_systemChangeStateLogger;
    }
}

} // namespace LoggingSystem
} // namespace Stp
