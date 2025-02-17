#pragma once

#include "State.hpp"

class Action {
public:
    int cost;
    virtual bool CanExecute(const State& state) = 0;
    virtual void Execute(State& state) = 0;
    virtual void changeCost(int newCost);
    virtual ~Action() {}
};

class EatAction : public Action {
public:
    EatAction(int _cost);
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
    void changeCost(int newCost) override;
};

class SearchFoodAction : public Action {
public:
    SearchFoodAction(int _cost);
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
    void changeCost(int newCost) override;
};