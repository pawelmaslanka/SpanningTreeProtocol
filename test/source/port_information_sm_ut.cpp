/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// Tested project's headers
#include <stp/sm/port_information.hpp>
// UT dependencies
#include "sut_machine.hpp"
#include <mock/logger.hpp>
#include <mock/management.hpp>
#include <mock/port_information_sm.hpp>

// GTest headers
#include <gtest/gtest.h>

using namespace Stp::PortInformation;

class PortInformationTest : public ::testing::Test {
protected:
    PortInformationTest()
        : _bridge{
              std::make_shared<Stp::Bridge>(
                  std::make_shared<Stp::System>(
                      std::make_shared<Mock::OutInterface>(),
                      std::make_shared<Mock::Logger>())) },
          _port{ std::make_shared<Stp::Port>() },
          _sutMachine{ _bridge, _port } {

    }

    Stp::BridgeH _bridge;
    Stp::PortH _port;
    SutMachine _sutMachine;
    Mock::Pim::BeginState _mockBeginState;
};

TEST_F(PortInformationTest,
       testBeginStateExecute_withFailedGoToDisabled_shouldNotChangeState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Pim::BeginState::RealExecute));
    EXPECT_CALL(_mockBeginState, GoToDisabled(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortInformation::BeginState::Instance().Name().c_str());
}

TEST_F(PortInformationTest,
       testBeginStateExecute_withSuccessedGoToDisabled_shouldChangeStateOntoDisabledState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Pim::BeginState::RealExecute));
    EXPECT_CALL(_mockBeginState, GoToDisabled(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockBeginState, DisabledAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockBeginState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Pim::BeginState::RealChangeState));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortInformation::DisabledState::Instance().Name().c_str());
}
