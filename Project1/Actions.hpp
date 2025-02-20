#pragma once

#include "State.hpp"

class Action {
public:
    virtual bool CanExecute(const State& state) = 0;
    virtual void Execute(State& state) = 0;
    virtual void changeCost(int newCost) = 0;
    virtual int  GetCost() = 0;
    virtual ~Action() {}
};

class PatrollingAction : public Action {
private:
    int cost;

public:
    PatrollingAction(int _cost);
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
    void changeCost(int newCost) override;
    int GetCost() override;
};

class ChaseAction : public Action {
private:
    int cost;

public:
    ChaseAction(int _cost);
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
    void changeCost(int newCost) override;
    int GetCost() override;
};

class SearchPlayerAction : public Action {
private:
    int cost;

public:
    SearchPlayerAction(int _cost);
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
    void changeCost(int newCost) override;
    int GetCost() override;
};

class FleeAction : public Action {
private:
    int cost;

public:
    FleeAction(int _cost);
    bool CanExecute(const State& state) override;
    void Execute(State& state) override;
    void changeCost(int newCost) override;
    int GetCost() override;
};