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
        TransitionToNewState = 1 << 1
    };
    Logger(const MsgSeverity severityLevel);
    virtual void operator<<(std::string&& msg) noexcept = 0;

protected:
    ~Logger() = default;

private:
    MsgSeverity _msgSeverityLevel;
};

} // namespace Stp
