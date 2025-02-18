#pragma once

#include "State.hpp"

class Action {
public:
    virtual bool CanExecute(const State& state) = 0;
    virtual void Execute(State& state) = 0;
    virtual void changeCost(int newCost) = 0;
    virtual ~Action() {}
};

class PatrollingAction : public Action {
public:
    int cost;

public:
    PatrollingAction(int _cost);
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
    void changeCost(int newCost) override;
};

class ChaseAction : public Action {
public:
    int cost;

public:
    ChaseAction(int _cost);
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
    void changeCost(int newCost) override;
};

class SearchPlayerAction : public Action {
public:
    int cost;

public:
    SearchPlayerAction(int _cost);
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
    void changeCost(int newCost) override;
};

class FleeAction : public Action {
public:
    int cost;

public:
    FleeAction(int _cost);
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
    void changeCost(int newCost) override;
};