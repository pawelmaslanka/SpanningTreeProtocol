#pragma once

// This project's headers
#include "lib.hpp"

// C++ Standard Library
#include <string>

namespace Stp {
namespace LoggingSystem {

class Logger {
public:
    enum class LogSeverity : u8 {
        None,
        EntryState,
        ChangeState
    };
    Logger(const LogSeverity severityLevel = LogSeverity::None);
    virtual void operator<<(std::string&& msg) noexcept = 0;

protected:
    ~Logger() = default;

private:
    LogSeverity _logSeverity;
};

using LoggerH = Sptr<Logger>;

class SystemLoggingManager;

class EntryState {
protected:
    friend class SystemLoggingManager;
    EntryState(Logger& logger);
    virtual void Log(const char* machineName, const char* stateName) = 0;
    virtual ~EntryState() = default;
    Logger& _logger;
};

class NullEntryState : public EntryState {
protected:
    friend class SystemLoggingManager;
    NullEntryState(Logger& logger);
    void Log(const char* machineName, const char* stateName) override;
};

class SystemEntryState : public EntryState {
protected:
    friend class SystemLoggingManager;
    SystemEntryState(Logger& logger);
    void Log(const char* machineName, const char* stateName) override;
};

class ChangeState {
protected:
    friend class SystemLoggingManager;
    ChangeState(Logger& logger);
    virtual void Log(const char* machineName, const char* oldStateName, const char* newStateName) = 0;
    virtual ~ChangeState() = default;
    Logger& _logger;
};

class NullChangeState : public ChangeState {
protected:
    friend class SystemLoggingManager;
    NullChangeState(Logger& logger);
    virtual void Log(const char* machineName, const char* oldStateName,
                     const char* newStateName) override;
};

class SystemChangeState : public ChangeState {
protected:
    friend class SystemLoggingManager;
    SystemChangeState(Logger& logger);
    void Log(const char* machineName, const char* oldStateName, const char* newStateName) override;
};

class SystemLoggingManager {
public:
    SystemLoggingManager(LoggerH logger);

    __virtual void LogEntryState(const char* machineName, const char* stateName);
    __virtual void LogChangeState(const char* machineName, const char* oldStateName,
                                        const char* newStateName);
    __virtual void SetLogSeverity(const Logger::LogSeverity logSeverity);

private:
    LoggerH _logger;
    EntryState* _entryStateLogger;
    ChangeState* _changeStateLogger;

    NullEntryState _nullEntryStateLogger;
    SystemEntryState _systemEntryStateLogger;
    NullChangeState _nullChangeStateLogger;
    SystemChangeState _systemChangeStateLogger;
};

inline void SystemLoggingManager::LogEntryState(const char* machineName,
                                                      const char* stateName) {
    _entryStateLogger->Log(machineName, stateName);
}

inline void SystemLoggingManager::LogChangeState(const char* machineName,
                                                       const char* oldStateName,
                                                       const char* newStateName) {
    _changeStateLogger->Log(machineName, oldStateName, newStateName);
}

inline EntryState::EntryState(Logger& logger)
    : _logger { logger } {
}

inline NullEntryState::NullEntryState(Logger& logger)
    : EntryState { logger } {
}

inline void NullEntryState::Log(const char*, const char*) {
}

inline SystemEntryState::SystemEntryState(Logger& logger)
    : EntryState{ logger } {
}

inline ChangeState::ChangeState(Logger& logger)
    : _logger{ logger } {
}

inline NullChangeState::NullChangeState(Logger& logger)
    : ChangeState{ logger } {
}

inline void NullChangeState::Log(const char*, const char*, const char*) {
}

inline SystemChangeState::SystemChangeState(Logger& logger)
    : ChangeState{ logger } {
}

} // namespace LoggingSystem
} // namespace Stp
