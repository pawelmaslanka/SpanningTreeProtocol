#pragma once

// This project's headers
#include "lib.hpp"

// C++ Standard Library
#include <string>

namespace Stp {

// Make a decorator from logger
class Logger {
public:
    enum class MsgSeverity : u8 {
        None = 0x00,
        StateEntry = 1 << 0,
        ChangeState = 1 << 1
    };
    Logger(const MsgSeverity severityLevel = MsgSeverity::None);
    virtual void operator<<(std::string&& msg) noexcept = 0;

protected:
    ~Logger() = default;

private:
    MsgSeverity _msgSeverityLevel;
};

using LoggerH = Sptr<Logger>;

class StateEntryLogger {
public:
    StateEntryLogger(LoggerH logger);
    virtual void Log(const char* machineName, const char* stateName) = 0;

protected:
    virtual ~StateEntryLogger() = default;
    LoggerH _logger;
};

class NullStateEntryLogger : public StateEntryLogger {
public:
    NullStateEntryLogger(LoggerH logger);
    void Log(const char* machineName, const char* stateName) override;
};

class SystemStateEntryLogger : public StateEntryLogger {
public:
    SystemStateEntryLogger(LoggerH logger);
    void Log(const char* machineName, const char* stateName) override;
};

class ChangeStateLogger {
public:
    ChangeStateLogger(LoggerH logger);
    virtual void Log(const char* machineName, const char* oldStateName, const char* newStateName) = 0;

protected:
    virtual ~ChangeStateLogger() = default;
    LoggerH _logger;
};

class NullChangeStateLogger : public ChangeStateLogger {
public:
    NullChangeStateLogger(LoggerH logger);
    virtual void Log(const char* machineName, const char* oldStateName,
                     const char* newStateName) override;
};

class SystemChangeStateLogger : public ChangeStateLogger {
public:
    SystemChangeStateLogger(LoggerH logger);
    void Log(const char* machineName, const char* oldStateName, const char* newStateName) override;
};

inline StateEntryLogger::StateEntryLogger(LoggerH logger)
    : _logger { logger } {
}

inline NullStateEntryLogger::NullStateEntryLogger(LoggerH logger)
    : StateEntryLogger { logger } {
}

inline void NullStateEntryLogger::Log(const char*, const char*) {
}

inline SystemStateEntryLogger::SystemStateEntryLogger(LoggerH logger)
    : StateEntryLogger{ logger } {
}

inline ChangeStateLogger::ChangeStateLogger(LoggerH logger)
    : _logger{ logger } {
}

inline NullChangeStateLogger::NullChangeStateLogger(LoggerH logger)
    : ChangeStateLogger{ logger } {
}

inline void NullChangeStateLogger::Log(const char*, const char*, const char*) {
}

inline SystemChangeStateLogger::SystemChangeStateLogger(LoggerH logger)
    : ChangeStateLogger{ logger } {
}

} // namespace Stp
