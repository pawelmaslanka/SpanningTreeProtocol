// Tested project's headers
#include "port_protocol_migration_sm.hpp"
// UT dependencies
#include <mock/port_protocol_migration_sm.hpp>

// GTest headers
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::Exactly;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::_; // Matcher for parameters

using namespace SpanningTree::PortProtocolMigration;

class Sut : public Machine {
public:
    Sut(SpanningTree::BridgeH bridge, SpanningTree::PortH port)
        : Machine{ bridge, port } {}
    void ChangeState(StateH newState) { Machine::ChangeState(newState); }
};

class PortProtocolMigrationTest : public ::testing::Test {
public:
    PortProtocolMigrationTest() {
        // Initialization code here
    }

protected:
    void SetUp() {
        // Code here will execute just before the test ensues
        _sut = new Sut{ _bridge, _port };
        _mockBeginState = new Mock::Ppm::BeginState;
        _mockCheckingRstpState = new Mock::Ppm::CheckingRstpState;
        _mockSensingState = new Mock::Ppm::SensingState;
        _mockSelectingStpState = new Mock::Ppm::SelectingStpState;
    }

    void TearDown() {
        // Code here will be called just after the test completes
        // Ok to through exceptions from here if need be
        delete _mockSelectingStpState;
        delete _mockSensingState;
        delete _mockCheckingRstpState;
        delete _mockBeginState;
        delete _sut;
    }

    ~PortProtocolMigrationTest()  {
        // Cleanup any pending stuff, but no exceptions allowed
    }

    // Put in any custom data members that you need
    using SutH = Sut&;
    using MockBeginStateH = Mock::Ppm::BeginState&;
    using MockCheckingRstpStateH = Mock::Ppm::CheckingRstpState&;
    using MockSensingStateH = Mock::Ppm::SensingState&;
    using MockSelectingStpStateH = Mock::Ppm::SelectingStpState&;

    SpanningTree::Bridge _bridge;
    SpanningTree::Port _port;

    SutH SutInstance() { return *_sut; }
    MockBeginStateH MockBeginStateInstance() { return *_mockBeginState; }
    MockCheckingRstpStateH MockCheckingRstpStateInstance() { return *_mockCheckingRstpState; }
    MockSensingStateH MockSensingStateInstance() { return *_mockSensingState; }
    MockSelectingStpStateH MockSelectingStpStateInstance() { return *_mockSelectingStpState; }

private:
    Sut* _sut;
    Mock::Ppm::BeginState* _mockBeginState;
    Mock::Ppm::CheckingRstpState* _mockCheckingRstpState;
    Mock::Ppm::SensingState* _mockSensingState;
    Mock::Ppm::SelectingStpState* _mockSelectingStpState;
};

TEST_F(PortProtocolMigrationTest,
       testBeginStateExecute_withFailedGoToCheckingRstp_shouldNotChangeState) {
    EXPECT_CALL(MockBeginStateInstance(), Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&MockBeginStateInstance(), &Mock::Ppm::BeginState::RealExecute));
    EXPECT_CALL(MockBeginStateInstance(), GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    SutInstance().ChangeState(MockBeginStateInstance());
    SutInstance().Run();
}

TEST_F(PortProtocolMigrationTest,
       testBeginStateExecute_withSucceedGoToCheckingRstp_shouldChangeStateOntoCheckingRstpState) {
    EXPECT_CALL(MockBeginStateInstance(), Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&MockBeginStateInstance(), &Mock::Ppm::BeginState::RealExecute));
    EXPECT_CALL(MockBeginStateInstance(), GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(MockBeginStateInstance(), CheckingRstpAction(_)).Times(Exactly(1));
    EXPECT_CALL(MockBeginStateInstance(), ChangeState(_, _)).Times(Exactly(1));

    SutInstance().ChangeState(MockBeginStateInstance());
    SutInstance().Run();
}

TEST_F(PortProtocolMigrationTest,
       testCheckingRstpStateExecute_withFailedGoToCheckingRstpAndFailedGoToSensing_shouldNotChangeState) {
    EXPECT_CALL(MockCheckingRstpStateInstance(), Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&MockCheckingRstpStateInstance(),
                             &Mock::Ppm::CheckingRstpState::RealExecute));
    EXPECT_CALL(MockCheckingRstpStateInstance(), GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(MockCheckingRstpStateInstance(), GoToSensing(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    SutInstance().ChangeState(MockCheckingRstpStateInstance());
    SutInstance().Run();
}

TEST_F(PortProtocolMigrationTest,
       testCheckingRstpStateExecute_withSucceedGoToSensing_shouldChangeStateOntoSensingState) {
    EXPECT_CALL(MockCheckingRstpStateInstance(), Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&MockCheckingRstpStateInstance(),
                             &Mock::Ppm::CheckingRstpState::RealExecute));
    EXPECT_CALL(MockCheckingRstpStateInstance(), GoToSensing(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(MockCheckingRstpStateInstance(), SensingAction(_)).Times(Exactly(1));
    EXPECT_CALL(MockCheckingRstpStateInstance(), ChangeState(_, _)).Times(Exactly(1));

    SutInstance().ChangeState(MockCheckingRstpStateInstance());
    SutInstance().Run();
}

TEST_F(PortProtocolMigrationTest,
       testCheckingRstpStateExecute_withSucceedGoToCheckingRstp_shouldStayAtCheckingRstpState) {
    EXPECT_CALL(MockCheckingRstpStateInstance(), Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&MockCheckingRstpStateInstance(),
                             &Mock::Ppm::CheckingRstpState::RealExecute));
    EXPECT_CALL(MockCheckingRstpStateInstance(), GoToSensing(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(MockCheckingRstpStateInstance(), GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(MockCheckingRstpStateInstance(), CheckingRstpAction(_)).Times(Exactly(1));

    SutInstance().ChangeState(MockCheckingRstpStateInstance());
    SutInstance().Run();
}

TEST_F(PortProtocolMigrationTest,
       testSensingStateExecute_withFailedGoToCheckingRstpAndFailedGoToSelectingStp_shouldNotChangeState) {
    EXPECT_CALL(MockSensingStateInstance(), Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&MockSensingStateInstance(),
                             &Mock::Ppm::SensingState::RealExecute));
    EXPECT_CALL(MockSensingStateInstance(), GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(MockSensingStateInstance(), GoToSelectingStp(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    SutInstance().ChangeState(MockSensingStateInstance());
    SutInstance().Run();
}

TEST_F(PortProtocolMigrationTest,
       testSensingStateExecute_withSucceedGoToCheckingRstp_shouldChangeStateOntoCheckingRstpState) {
    EXPECT_CALL(MockSensingStateInstance(), Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&MockSensingStateInstance(),
                             &Mock::Ppm::SensingState::RealExecute));
    EXPECT_CALL(MockSensingStateInstance(), GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(MockSensingStateInstance(), CheckingRstpAction(_)).Times(Exactly(1));
    EXPECT_CALL(MockSensingStateInstance(), ChangeState(_, _)).Times(Exactly(1));

    SutInstance().ChangeState(MockSensingStateInstance());
    SutInstance().Run();
}

TEST_F(PortProtocolMigrationTest,
       testSensingStateExecute_withSucceedGoToSelectingStp_shouldChangeStateOntoSelectingStpState) {
    EXPECT_CALL(MockSensingStateInstance(), Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&MockSensingStateInstance(),
                             &Mock::Ppm::SensingState::RealExecute));
    EXPECT_CALL(MockSensingStateInstance(), GoToCheckingRstp(_)).Times(Exactly(1))
            .WillOnce(Return(false));
    EXPECT_CALL(MockSensingStateInstance(), GoToSelectingStp(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(MockSensingStateInstance(), SelectingStpAction(_)).Times(Exactly(1));
    EXPECT_CALL(MockSensingStateInstance(), ChangeState(_, _)).Times(Exactly(1));

    SutInstance().ChangeState(MockSensingStateInstance());
    SutInstance().Run();
}

TEST_F(PortProtocolMigrationTest,
       testSelectingStpStateExecute_withFailedGoToSensing_shouldNotChangeState) {
    EXPECT_CALL(MockSelectingStpStateInstance(), Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&MockSelectingStpStateInstance(),
                             &Mock::Ppm::SelectingStpState::RealExecute));
    EXPECT_CALL(MockSelectingStpStateInstance(), GoToSensing(_)).Times(Exactly(1))
            .WillOnce(Return(false));

    SutInstance().ChangeState(MockSelectingStpStateInstance());
    SutInstance().Run();
}

TEST_F(PortProtocolMigrationTest,
       testSelectingStpStateExecute_withSucceedGoToSensing_shouldChangeStateOntoSensingState) {
    EXPECT_CALL(MockSelectingStpStateInstance(), Execute(_)).Times(Exactly(1))
            .WillOnce(Invoke(&MockSelectingStpStateInstance(),
                             &Mock::Ppm::SelectingStpState::RealExecute));
    EXPECT_CALL(MockSelectingStpStateInstance(), GoToSensing(_)).Times(Exactly(1))
            .WillOnce(Return(true));
    EXPECT_CALL(MockSelectingStpStateInstance(), SensingAction(_)).Times(Exactly(1));
    EXPECT_CALL(MockSelectingStpStateInstance(), ChangeState(_, _)).Times(Exactly(1));

    SutInstance().ChangeState(MockSelectingStpStateInstance());
    SutInstance().Run();
}
