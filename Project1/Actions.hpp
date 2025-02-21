#pragma once

#include "State.hpp"

class Action {
public:
    virtual bool CanExecute(const Action& action, State& state) = 0;
    virtual void Execute(State& state, const string& action) = 0;
    virtual void changeCost(int newCost) = 0;
    virtual int  GetCost() = 0;
    virtual ~Action() {}

public:
    int cost;
    string id;
    vector<string> conditions;
    vector<string> effects;
};

class SpecificAction : public Action {
public:
    // Setters
    static void applyAction(const Action& action, State& state);
    void changeCost(int newCost) override;
    void Execute(State& state, const string& action) override;
    
    // Getters
    bool CanExecute(const Action& action, State& state) override;
    int GetCost() override;
};

class PatrollingAction : public SpecificAction {
public:
    PatrollingAction(int _cost, const vector<string>& conditions, const vector<string>& effects, const string& id);
};

class ChaseAction : public SpecificAction {
public:
    ChaseAction(int _cost, const vector<string>& conditions, const vector<string>& effects, const string& id);
};

class AttackAction : public SpecificAction {
public:
    AttackAction(int _cost, const vector<string>& conditions, const vector<string>& effects, const string& id);
};

class FleeAction : public SpecificAction {
public:
    FleeAction(int _cost, const vector<string>& conditions, const vector<string>& effects, const string& id);
};