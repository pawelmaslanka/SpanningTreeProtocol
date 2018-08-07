// Tested project's headers
#include "port_protocol_migration_sm.hpp"
// UT dependencies
#include <mock/port_receive_sm.hpp>
#include "sut_machine.hpp"

// GTest headers
#include <gtest/gtest.h>

using namespace SpanningTree::PortReceive;

class PortReceiveTest : public ::testing::Test {
protected:
    PortReceiveTest() : _sutMachine{ _bridge, _port } {}

    SpanningTree::Bridge _bridge;
    SpanningTree::Port _port;
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
                 SpanningTree::PortReceive::kBeginStateName);
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
                 SpanningTree::PortReceive::kDiscardStateName);
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
                 SpanningTree::PortReceive::kDiscardStateName);
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
                 SpanningTree::PortReceive::kReceiveStateName);
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
                 SpanningTree::PortReceive::kReceiveStateName);
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
                 SpanningTree::PortReceive::kReceiveStateName);
}
