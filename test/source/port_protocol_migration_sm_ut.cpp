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
#include <mock/port_protocol_migration_sm.hpp>

// GTest headers
#include <gtest/gtest.h>

using ::testing::Exactly;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::_; // Matcher for parameters

using namespace Stp::PortProtocolMigration;

class PortProtocolMigrationTest : public ::testing::Test {
protected:
    PortProtocolMigrationTest()
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
    Mock::Ppm::BeginState _mockBeginState;
    Mock::Ppm::CheckingRstpState _mockCheckingRstpState;
    Mock::Ppm::SelectingStpState _mockSelectingStpState;
    Mock::Ppm::SensingState _mockSensingState;
};

TEST_F(PortProtocolMigrationTest,
       testBeginStateExecute_withFailedGoToCheckingRstp_shouldNotChangeState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Ppm::BeginState::RealExecute));
    EXPECT_CALL(_mockBeginState, GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortProtocolMigration::BeginState::Instance().Name().c_str());
}

TEST_F(PortProtocolMigrationTest,
       testBeginStateExecute_withSucceedGoToCheckingRstp_shouldChangeStateOntoCheckingRstpState) {
    EXPECT_CALL(_mockBeginState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Ppm::BeginState::RealExecute));
    EXPECT_CALL(_mockBeginState, GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockBeginState, CheckingRstpAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockBeginState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockBeginState, &Mock::Ppm::BeginState::RealChangeState));

    _sutMachine.ChangeState(_mockBeginState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortProtocolMigration::CheckingRstpState::Instance().Name().c_str());
}

TEST_F(PortProtocolMigrationTest,
       testCheckingRstpStateExecute_withFailedAllTransitions_shouldNotChangeState) {
    EXPECT_CALL(_mockCheckingRstpState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockCheckingRstpState,
                             &Mock::Ppm::CheckingRstpState::RealExecute));
    EXPECT_CALL(_mockCheckingRstpState, GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockCheckingRstpState, GoToSensing(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockCheckingRstpState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortProtocolMigration::CheckingRstpState::Instance().Name().c_str());
}

TEST_F(PortProtocolMigrationTest,
       testCheckingRstpStateExecute_withSucceedGoToSensing_shouldChangeStateOntoSensingState) {
    EXPECT_CALL(_mockCheckingRstpState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockCheckingRstpState,
                             &Mock::Ppm::CheckingRstpState::RealExecute));
    EXPECT_CALL(_mockCheckingRstpState, GoToSensing(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockCheckingRstpState, SensingAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockCheckingRstpState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockCheckingRstpState, &Mock::Ppm::CheckingRstpState::RealChangeState));

    _sutMachine.ChangeState(_mockCheckingRstpState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortProtocolMigration::SensingState::Instance().Name().c_str());
}

TEST_F(PortProtocolMigrationTest,
       testCheckingRstpStateExecute_withSucceedGoToCheckingRstp_shouldStayAtCheckingRstpState) {
    EXPECT_CALL(_mockCheckingRstpState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockCheckingRstpState,
                             &Mock::Ppm::CheckingRstpState::RealExecute));
    EXPECT_CALL(_mockCheckingRstpState, GoToSensing(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockCheckingRstpState, GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockCheckingRstpState, CheckingRstpAction(_)).Times(Exactly(1));

    _sutMachine.ChangeState(_mockCheckingRstpState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortProtocolMigration::CheckingRstpState::Instance().Name().c_str());
}

TEST_F(PortProtocolMigrationTest,
       testSensingStateExecute_withFailedGoToCheckingRstpAndFailedGoToSelectingStp_shouldNotChangeState) {
    EXPECT_CALL(_mockSensingState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockSensingState,
                             &Mock::Ppm::SensingState::RealExecute));
    EXPECT_CALL(_mockSensingState, GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockSensingState, GoToSelectingStp(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockSensingState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortProtocolMigration::SensingState::Instance().Name().c_str());
}

TEST_F(PortProtocolMigrationTest,
       testSensingStateExecute_withSucceedGoToCheckingRstp_shouldChangeStateOntoCheckingRstpState) {
    EXPECT_CALL(_mockSensingState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockSensingState,
                             &Mock::Ppm::SensingState::RealExecute));
    EXPECT_CALL(_mockSensingState, GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockSensingState, CheckingRstpAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockSensingState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockSensingState, &Mock::Ppm::SensingState::RealChangeState));

    _sutMachine.ChangeState(_mockSensingState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortProtocolMigration::CheckingRstpState::Instance().Name().c_str());
}

TEST_F(PortProtocolMigrationTest,
       testSensingStateExecute_withSucceedGoToSelectingStp_shouldChangeStateOntoSelectingStpState) {
    EXPECT_CALL(_mockSensingState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockSensingState,
                             &Mock::Ppm::SensingState::RealExecute));
    EXPECT_CALL(_mockSensingState, GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(_mockSensingState, GoToSelectingStp(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockSensingState, SelectingStpAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockSensingState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockSensingState, &Mock::Ppm::SensingState::RealChangeState));

    _sutMachine.ChangeState(_mockSensingState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortProtocolMigration::SelectingStpState::Instance().Name().c_str());
}

TEST_F(PortProtocolMigrationTest,
       testSelectingStpStateExecute_withFailedGoToSensing_shouldNotChangeState) {
    EXPECT_CALL(_mockSelectingStpState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockSelectingStpState,
                             &Mock::Ppm::SelectingStpState::RealExecute));
    EXPECT_CALL(_mockSelectingStpState, GoToSensing(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    _sutMachine.ChangeState(_mockSelectingStpState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortProtocolMigration::SelectingStpState::Instance().Name().c_str());
}

TEST_F(PortProtocolMigrationTest,
       testSelectingStpStateExecute_withSucceedGoToSensing_shouldChangeStateOntoSensingState) {
    EXPECT_CALL(_mockSelectingStpState, Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockSelectingStpState,
                             &Mock::Ppm::SelectingStpState::RealExecute));
    EXPECT_CALL(_mockSelectingStpState, GoToSensing(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(_mockSelectingStpState, SensingAction(_)).Times(Exactly(1));
    EXPECT_CALL(_mockSelectingStpState, ChangeState(_, _)).Times(Exactly(1))
            .WillOnce(Invoke(&_mockSelectingStpState, &Mock::Ppm::SelectingStpState::RealChangeState));

    _sutMachine.ChangeState(_mockSelectingStpState);
    _sutMachine.Run();

    EXPECT_STREQ(_sutMachine.CurrentState().Name().c_str(),
                 Stp::PortProtocolMigration::SensingState::Instance().Name().c_str());
}
