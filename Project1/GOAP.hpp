#pragma once

#include "Actions.hpp"
#include "State.hpp"

enum class Goal {
    Manger,
    ChercherNourriture
};

class GOAPPlanner {
public:
    std::vector<std::unique_ptr<Action>> Plan(const State& initialState, Goal& goal);
};

class GOAPAgent {
private:
    State state;
    GOAPPlanner planner;

public:
    GOAPAgent();

public:
    void PerformActions();
    void PrintState();
};