/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// Tested project's headers
#include <stp/management.hpp>

// GTest headers
#include <gmock/gmock.h>

namespace Mock {

class OutInterface : public Stp::OutInterface {
public:
    MOCK_METHOD1(FlushFdb, Stp::Result(const Stp::u16));
    MOCK_METHOD2(SetForwarding, Stp::Result(const Stp::u16, const bool));
    MOCK_METHOD2(SetLearning, Stp::Result(const Stp::u16, const bool));
    MOCK_METHOD2(SendOutBpdu, Stp::Result(const Stp::u16, Stp::ByteStreamH));
};

} // namespace Mock
