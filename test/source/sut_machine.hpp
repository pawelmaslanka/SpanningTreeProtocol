/**
 * @author Pawel Maslanka (pawmas)
 *
 * Contact: pawmas@hotmail.com
 */

#pragma once

// This project's headers
#include <stp/state_machine.hpp>

// GTest headers
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_; // Matcher for parameters
using ::testing::Exactly;
using ::testing::Invoke;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;

class SutMachine : public Stp::Machine {
public:
    SutMachine(Stp::BridgeH bridge, Stp::PortH port)
        : Machine{ bridge, port, _dummyBeginState } {}
    void ChangeState(Stp::State& newState) { Machine::ChangeState(newState); }
    Stp::State& CurrentState() const noexcept { return Stp::Machine::CurrentState(); }
    std::string Name() override { return "Machine SUT"; }

private:
    class BeginState : public Stp::State {
    public:
        void Execute(Stp::Machine& machine) override { std::ignore = machine; }
        std::string Name() override { return "Machine SUT Begin State"; }
    } _dummyBeginState;
};

using SutMachineH = SutMachine&;
