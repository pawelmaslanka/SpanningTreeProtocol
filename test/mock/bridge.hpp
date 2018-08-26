#pragma once

// Tested project's headers
#include <stp/bridge.hpp>
#include <stp/management.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {

class Bridge : public Stp::Bridge {
public:
    Bridge(Stp::OutInterfaceH outInterface) : Stp::Bridge{ outInterface } {}
    virtual ~Bridge() = default;
    MOCK_CONST_METHOD0(Begin, bool());
};

} // namespace Mock
