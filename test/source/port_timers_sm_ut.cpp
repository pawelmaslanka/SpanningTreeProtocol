// Tested project's headers
#include <stp/sm/port_timers.hpp>
// UT dependencies
#include "sut_machine.hpp"
#include <mock/management.hpp>
#include <mock/port_timers_sm.hpp>

// GTest headers
#include <gtest/gtest.h>

using namespace Stp::PortTimers;

class PortTimersTest : public ::testing::Test {
protected:
    PortTimersTest()
        : _bridge{ std::make_shared<Stp::Bridge>(
                       std::make_shared<Mock::OutInterface>()) },
          _port{ std::make_shared<Stp::Port>() },
          _sutMachine{ _bridge, _port } {}

    Stp::BridgeH _bridge;
    Stp::PortH _port;
    SutMachine _sutMachine;
    Mock::Pti::BeginState _mockBeginState;
    Mock::Pti::OneSecondState _mockOneSecondState;
    Mock::Pti::TickState _mockTickState;
};

TEST_F(PortTimersTest,
       testBeginStateExecute_withFailedGoToOneSecond_shouldNotChangeState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Pti::BeginState::RealExecute));
    EXPECT_CALL(_mockBeginState, GoToOneSecond(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTimers::kBeginStateName);
}

TEST_F(PortTimersTest,
       testBeginStateExecute_withSuccessedGoToOneSecond_shouldChangeStateOntoOneSecondState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Pti::BeginState::RealExecute));
    EXPECT_CALL(_mockBeginState, GoToOneSecond(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockBeginState, OneSecondAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockBeginState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Pti::BeginState::RealChangeState));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTimers::kOneSecondStateName);
}

TEST_F(PortTimersTest,
       testOneSecondStateExecute_withFailedGoToTick_shouldNotChangeState) {
    EXPECT_CALL(_mockOneSecondState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockOneSecondState, &Mock::Pti::OneSecondState::RealExecute));
    EXPECT_CALL(_mockOneSecondState, GoToTick(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockOneSecondState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTimers::kOneSecondStateName);
}

TEST_F(PortTimersTest,
       testOneSecondStateExecute_withSuccessedGoToOneSecond_shouldChangeStateOntoTickState) {
    EXPECT_CALL(_mockOneSecondState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockOneSecondState, &Mock::Pti::OneSecondState::RealExecute));
    EXPECT_CALL(_mockOneSecondState, GoToTick(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockOneSecondState, TickAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockOneSecondState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockOneSecondState, &Mock::Pti::OneSecondState::RealChangeState));

    _sutMachine.ChangeState(_mockOneSecondState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTimers::kTickStateName);
}

TEST_F(PortTimersTest,
       testTickStateExecute_withFailedGoToOneSecond_shouldNotChangeState) {
    EXPECT_CALL(_mockTickState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTickState, &Mock::Pti::TickState::RealExecute));
    EXPECT_CALL(_mockTickState, GoToOneSecond(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockTickState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTimers::kTickStateName);
}

TEST_F(PortTimersTest,
       testTickStateExecute_withSuccessedGoToOneSecond_shouldChangeStateOntoOneSecondState) {
    EXPECT_CALL(_mockTickState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTickState, &Mock::Pti::TickState::RealExecute));
    EXPECT_CALL(_mockTickState, GoToOneSecond(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockTickState, OneSecondAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockTickState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTickState, &Mock::Pti::TickState::RealChangeState));

    _sutMachine.ChangeState(_mockTickState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTimers::kOneSecondStateName);
}
