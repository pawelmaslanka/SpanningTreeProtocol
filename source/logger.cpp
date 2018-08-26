// This project's headers
#include "stp/logger.hpp"

namespace Stp {

Logger::Logger(Logger::MsgSeverity msgLogSeverity)
    : _msgSeverityLevel { msgLogSeverity } {
    // Nothing more to do
}

} // namespace Stp
