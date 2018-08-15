#pragma once

// Tested project's headers
#include <stp/bridge.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {

class Bridge : public Stp::Bridge {
public:
    MOCK_CONST_METHOD0(Begin, bool());
};

} // namespace Mock
