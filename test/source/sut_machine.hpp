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

class SutMachine : public SpanningTree::Machine {
public:
    SutMachine(sptr<SpanningTree::Bridge> bridge, sptr<SpanningTree::Port> port)
        : Machine{ bridge, port, _dummyBeginState } {}
    void ChangeState(SpanningTree::StateH newState) { Machine::ChangeState(newState); }
    SpanningTree::StateH CurrentState() const noexcept { return SpanningTree::Machine::CurrentState(); }

private:
    class BeginState : public SpanningTree::State {
    public:
        void Execute(SpanningTree::MachineH machine) override { std::ignore = machine; }
        std::string Name() override { return "Machine SUT Begin State"; }
    } _dummyBeginState;
};

using SutMachineH = SutMachine&;
