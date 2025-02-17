#pragma once

#include "State.hpp"

class Action {
public:
    virtual bool CanExecute(const State& state) = 0;
    virtual void Execute(State& state) = 0;
    virtual ~Action() {}
};

class EatAction : public Action {
public:
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
};

class SearchFoodAction : public Action {
public:
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
};