/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

// Tested project's headers
#include <stp/sm/port_transmit.hpp>
// UT dependencies
#include "sut_machine.hpp"
#include <mock/logger.hpp>
#include <mock/management.hpp>
#include <mock/port_transmit_sm.hpp>

// GTest headers
#include <gtest/gtest.h>

using namespace Stp::PortTransmit;

class PortTransmitTest : public ::testing::Test {
protected:
    PortTransmitTest()
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
    Mock::Ptx::BeginState _mockBeginState;
    Mock::Ptx::TransmitInitState _mockTransmitInitState;
    Mock::Ptx::TransmitPeriodicState _mockTransmitPeriodicState;
    Mock::Ptx::TransmitConfigState _mockTransmitConfigState;
    Mock::Ptx::TransmitTcnState _mockTransmitTcnState;
    Mock::Ptx::TransmitRstpState _mockTransmitRstpState;
    Mock::Ptx::IdleState _mockIdleState;
};

TEST_F(PortTransmitTest,
       testBeginStateExecute_withFailedGoToTransmitInit_shouldNotChangeState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Ptx::BeginState::RealExecute));
    EXPECT_CALL(_mockBeginState, GoToTransmitInit(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::BeginState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testBeginStateExecute_withSuccessedGoToTransmitInit_shouldChangeStateOntoTransmitInitState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Ptx::BeginState::RealExecute));
    EXPECT_CALL(_mockBeginState, GoToTransmitInit(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockBeginState, TransmitInitAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockBeginState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Ptx::BeginState::RealChangeState));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::TransmitInitState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testTransmitInitStateExecute_withFailedGoToIdle_shouldNotChangeState) {
    EXPECT_CALL(_mockTransmitInitState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitInitState, &Mock::Ptx::TransmitInitState::RealExecute));
    EXPECT_CALL(_mockTransmitInitState, GoToIdle(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockTransmitInitState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::TransmitInitState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testTransmitInitStateExecute_withSuccessedGoToIdle_shouldChangeStateOntoIdleState) {
    EXPECT_CALL(_mockTransmitInitState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitInitState, &Mock::Ptx::TransmitInitState::RealExecute));
    EXPECT_CALL(_mockTransmitInitState, GoToIdle(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockTransmitInitState, IdleAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockTransmitInitState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitInitState,
                             &Mock::Ptx::TransmitInitState::RealChangeState));

    _sutMachine.ChangeState(_mockTransmitInitState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::IdleState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testTransmitPeriodicStateExecute_withFailedGoToIdle_shouldNotChangeState) {
    EXPECT_CALL(_mockTransmitPeriodicState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitPeriodicState,
                             &Mock::Ptx::TransmitPeriodicState::RealExecute));
    EXPECT_CALL(_mockTransmitPeriodicState, GoToIdle(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockTransmitPeriodicState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::TransmitPeriodicState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testTransmitPeriodicStateExecute_withSuccessedGoToIdle_shouldChangeStateOntoIdleState) {
    EXPECT_CALL(_mockTransmitPeriodicState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitPeriodicState,
                             &Mock::Ptx::TransmitPeriodicState::RealExecute));
    EXPECT_CALL(_mockTransmitPeriodicState, GoToIdle(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockTransmitPeriodicState, IdleAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockTransmitPeriodicState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitPeriodicState,
                             &Mock::Ptx::TransmitPeriodicState::RealChangeState));

    _sutMachine.ChangeState(_mockTransmitPeriodicState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::IdleState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testTransmitConfigStateExecute_withFailedGoToIdle_shouldNotChangeState) {
    EXPECT_CALL(_mockTransmitConfigState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitConfigState,
                             &Mock::Ptx::TransmitConfigState::RealExecute));
    EXPECT_CALL(_mockTransmitConfigState, GoToIdle(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockTransmitConfigState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::TransmitConfigState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testTransmitConfigStateExecute_withSuccessedGoToIdle_shouldChangeStateOntoIdleState) {
    EXPECT_CALL(_mockTransmitConfigState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitConfigState,
                             &Mock::Ptx::TransmitConfigState::RealExecute));
    EXPECT_CALL(_mockTransmitConfigState, GoToIdle(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockTransmitConfigState, IdleAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockTransmitConfigState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitConfigState,
                             &Mock::Ptx::TransmitConfigState::RealChangeState));

    _sutMachine.ChangeState(_mockTransmitConfigState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::IdleState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testTransmitTcnStateExecute_withFailedGoToIdle_shouldNotChangeState) {
    EXPECT_CALL(_mockTransmitTcnState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitTcnState,
                             &Mock::Ptx::TransmitTcnState::RealExecute));
    EXPECT_CALL(_mockTransmitTcnState, GoToIdle(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockTransmitTcnState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::TransmitTcnState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testTransmitTcnStateExecute_withSuccessedGoToIdle_shouldChangeStateOntoIdleState) {
    EXPECT_CALL(_mockTransmitTcnState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitTcnState,
                             &Mock::Ptx::TransmitTcnState::RealExecute));
    EXPECT_CALL(_mockTransmitTcnState, GoToIdle(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockTransmitTcnState, IdleAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockTransmitTcnState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitTcnState,
                             &Mock::Ptx::TransmitTcnState::RealChangeState));

    _sutMachine.ChangeState(_mockTransmitTcnState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::IdleState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testTransmitRstpStateExecute_withFailedGoToIdle_shouldNotChangeState) {
    EXPECT_CALL(_mockTransmitRstpState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitRstpState,
                             &Mock::Ptx::TransmitRstpState::RealExecute));
    EXPECT_CALL(_mockTransmitRstpState, GoToIdle(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockTransmitRstpState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::TransmitRstpState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testTransmitRstpStateExecute_withSuccessedGoToIdle_shouldChangeStateOntoIdleState) {
    EXPECT_CALL(_mockTransmitRstpState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitRstpState,
                             &Mock::Ptx::TransmitRstpState::RealExecute));
    EXPECT_CALL(_mockTransmitRstpState, GoToIdle(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockTransmitRstpState, IdleAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockTransmitRstpState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockTransmitRstpState,
                             &Mock::Ptx::TransmitRstpState::RealChangeState));

    _sutMachine.ChangeState(_mockTransmitRstpState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::IdleState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testIdleStateExecute_withFailedAllTransitions_shouldNotChangeState) {
    EXPECT_CALL(_mockIdleState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockIdleState,
                             &Mock::Ptx::IdleState::RealExecute));
    EXPECT_CALL(_mockIdleState, GoToTransmitPeriodic(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockIdleState, GoToTransmitConfig(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockIdleState, GoToTransmitTcn(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockIdleState, GoToTransmitRstp(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockIdleState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::IdleState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testIdleStateExecute_withSuccessedGoToTransmitPeriodic_shouldChangeStateOntoTransmitPeriodicState) {
    EXPECT_CALL(_mockIdleState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockIdleState,
                             &Mock::Ptx::IdleState::RealExecute));
    EXPECT_CALL(_mockIdleState, GoToTransmitPeriodic(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockIdleState, TransmitPeriodicAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockIdleState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockIdleState,
                             &Mock::Ptx::IdleState::RealChangeState));

    _sutMachine.ChangeState(_mockIdleState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::TransmitPeriodicState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testIdleStateExecute_withSuccessedGoToTransmitConfig_shouldChangeStateOntoTransmitConfigState) {
    EXPECT_CALL(_mockIdleState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockIdleState,
                             &Mock::Ptx::IdleState::RealExecute));
    EXPECT_CALL(_mockIdleState, GoToTransmitPeriodic(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockIdleState, GoToTransmitConfig(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockIdleState, TransmitConfigAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockIdleState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockIdleState,
                             &Mock::Ptx::IdleState::RealChangeState));

    _sutMachine.ChangeState(_mockIdleState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::TransmitConfigState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testIdleStateExecute_withSuccessedGoToTransmitTcn_shouldChangeStateOntoTransmitTcnState) {
    EXPECT_CALL(_mockIdleState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockIdleState,
                             &Mock::Ptx::IdleState::RealExecute));
    EXPECT_CALL(_mockIdleState, GoToTransmitPeriodic(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockIdleState, GoToTransmitConfig(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockIdleState, GoToTransmitTcn(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockIdleState, TransmitTcnAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockIdleState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockIdleState,
                             &Mock::Ptx::IdleState::RealChangeState));

    _sutMachine.ChangeState(_mockIdleState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::TransmitTcnState::Instance().Name().c_str());
}

TEST_F(PortTransmitTest,
       testIdleStateExecute_withSuccessedGoToTransmitRstp_shouldChangeStateOntoTransmitRstpState) {
    EXPECT_CALL(_mockIdleState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockIdleState,
                             &Mock::Ptx::IdleState::RealExecute));
    EXPECT_CALL(_mockIdleState, GoToTransmitPeriodic(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockIdleState, GoToTransmitConfig(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockIdleState, GoToTransmitTcn(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockIdleState, GoToTransmitRstp(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockIdleState, TransmitRstpAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockIdleState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockIdleState,
                             &Mock::Ptx::IdleState::RealChangeState));

    _sutMachine.ChangeState(_mockIdleState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortTransmit::TransmitRstpState::Instance().Name().c_str());
}
