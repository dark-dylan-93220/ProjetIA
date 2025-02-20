#pragma once

#include "Actions.hpp"
#include "State.hpp"

enum class Goal {
    Patrouiller,
    Chasser,
    Chercher,
    Fuir
};

class GOAPPlanner {
public:
    std::vector<std::unique_ptr<Action>> Plan(const State& initialState, std::vector<Goal>& goal);
};

class GOAPAgent {
private:
    State state;
    GOAPPlanner planner;

public:
    GOAPAgent();

public:
    void PerformActions(std::vector<Goal>& goals);
    void PrintState();
};