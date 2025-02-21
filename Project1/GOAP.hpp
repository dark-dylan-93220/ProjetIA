#pragma once

#include "Actions.hpp"
#include "State.hpp"
#include "Enemy.hpp"

class GOAPPlanner {
public:
    vector<SpecificAction> Plan(const State& initialState, State& goalState, vector<SpecificAction>& actions);
};

class GOAPAgent {
private:
    GOAPPlanner planner;

public:
    GOAPAgent();
    State state = State({});

public:
    void PerformActions(State& goalState, vector<SpecificAction>& actions, Enemy& enemy, const float& deltaTime);
    void PrintState(State& currentState);
};