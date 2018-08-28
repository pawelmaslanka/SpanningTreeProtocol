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

class StateEntryLogger : public Logger {
public:
    StateEntryLogger(LoggerH logger);
    virtual ~StateEntryLogger() = default;
    void operator<<(std::string&& msg) noexcept override;

private:
    LoggerH _logger;
};

class NullStateEntryLogger : public Logger {
public:
    NullStateEntryLogger(LoggerH logger);
    virtual ~NullStateEntryLogger() = default;
    void operator<<(std::string&& msg) noexcept override;
};

class ChangeStateLogger : public Logger {
public:
    ChangeStateLogger(LoggerH logger);
    virtual ~ChangeStateLogger() = default;
    void operator<<(std::string&& msg) noexcept override;

private:
    LoggerH _logger;
};

class NullChangeStateLogger : public Logger {
public:
    NullChangeStateLogger(LoggerH logger);
    virtual ~NullChangeStateLogger() = default;
    void operator<<(std::string&& msg) noexcept override;
};

inline StateEntryLogger::StateEntryLogger(LoggerH logger)
    : _logger{ logger } {
}

inline void StateEntryLogger::operator<<(std::string&& msg) noexcept {
    *_logger << std::move(msg);
}

inline ChangeStateLogger::ChangeStateLogger(LoggerH logger)
    : _logger{ logger } {
}

inline void ChangeStateLogger::operator<<(std::string&& msg) noexcept {
    *_logger << std::move(msg);
}

inline void NullStateEntryLogger::operator<<(std::string&& msg) noexcept {
    std::ignore = msg;
}

inline void NullChangeStateLogger::operator<<(std::string&& msg) noexcept {
    std::ignore = msg;
}

} // namespace Stp
