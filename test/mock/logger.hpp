/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// Tested project's headers
#include <stp/logger.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {

class Logger : public Stp::LoggingSystem::Logger {
public:
    MOCK_METHOD1(StreamOutOperator, void(std::string&&));
    void operator<<(std::string&& msg) noexcept override { std::move(msg); }
};

} // namespace Mock
