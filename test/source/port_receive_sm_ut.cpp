/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// Tested project's headers
#include <stp/sm/port_protocol_migration.hpp>
// UT dependencies
#include "sut_machine.hpp"
#include <mock/logger.hpp>
#include <mock/management.hpp>
#include <mock/port_receive_sm.hpp>

// GTest headers
#include <gtest/gtest.h>

using namespace Stp::PortReceive;

class PortReceiveTest : public ::testing::Test {
protected:
    PortReceiveTest()
        : _bridge{
              std::make_shared<Stp::Bridge>(
                  std::make_shared<Stp::System>(
                      std::make_shared<Mock::OutInterface>(),
                      std::make_shared<Mock::Logger>())) },
          _port{ std::make_shared<Stp::Port>() },
          _sutMachine{ _bridge, _port } {}

    Stp::BridgeH _bridge;
    Stp::PortH _port;
    SutMachine _sutMachine;
    Mock::Prx::BeginState _mockBeginState;
    Mock::Prx::DiscardState _mockDiscardState;
    Mock::Prx::ReceiveState _mockReceiveState;
};

TEST_F(PortReceiveTest,
       testBeginStateExecute_withFailedGoToDiscard_shouldNotChangeState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Prx::BeginState::RealExecute));
    EXPECT_CALL(_mockBeginState, GoToDiscard(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortReceive::BeginState::Instance().Name().c_str());
}

TEST_F(PortReceiveTest,
       testBeginStateExecute_withSucceededGoToDiscard_shouldChangeStateOntoDiscardState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Prx::BeginState::RealExecute));
    EXPECT_CALL(_mockBeginState, GoToDiscard(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockBeginState, DiscardAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockBeginState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Prx::BeginState::RealChangeState));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortReceive::DiscardState::Instance().Name().c_str());
}

TEST_F(PortReceiveTest,
       testDiscardStateExecute_withFailedGoToReceive_shouldNotChangeState) {
    EXPECT_CALL(_mockDiscardState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockDiscardState, &Mock::Prx::DiscardState::RealExecute));
    EXPECT_CALL(_mockDiscardState, GoToReceive(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockDiscardState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortReceive::DiscardState::Instance().Name().c_str());
}

TEST_F(PortReceiveTest,
       testDiscardStateExecute_withSucceededGoToReceive_shouldChangeStateOntoReceiveState) {
    EXPECT_CALL(_mockDiscardState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockDiscardState, &Mock::Prx::DiscardState::RealExecute));
    EXPECT_CALL(_mockDiscardState, GoToReceive(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockDiscardState, ReceiveAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockDiscardState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockDiscardState, &Mock::Prx::DiscardState::RealChangeState));

    _sutMachine.ChangeState(_mockDiscardState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortReceive::ReceiveState::Instance().Name().c_str());
}

TEST_F(PortReceiveTest,
       testReceiveStateExecute_withFailedGoToReceive_shouldNotChangeState) {
    EXPECT_CALL(_mockReceiveState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockReceiveState, &Mock::Prx::ReceiveState::RealExecute));
    EXPECT_CALL(_mockReceiveState, GoToReceive(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockReceiveState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortReceive::ReceiveState::Instance().Name().c_str());
}

TEST_F(PortReceiveTest,
       testReceiveStateExecute_withSucceedGoToReceive_shouldNotChangeState) {
    EXPECT_CALL(_mockReceiveState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockReceiveState, &Mock::Prx::ReceiveState::RealExecute));
    EXPECT_CALL(_mockReceiveState, GoToReceive(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockReceiveState, ReceiveAction(_)).Times(Exactly(1));

    _sutMachine.ChangeState(_mockReceiveState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortReceive::ReceiveState::Instance().Name().c_str());
}
