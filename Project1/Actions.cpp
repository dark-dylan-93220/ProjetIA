#include "Actions.hpp"
#include "State.hpp"


void SpecificAction::applyAction(const Action& action, State& state) {
    for (const auto& effect : action.effects) {
        state.addProperty(effect);
    }
    for (const auto& condition : action.conditions) {
        state.removeProperty(condition);
    }
}

// --- ACTION FUNCTIONS --- //
bool SpecificAction::CanExecute(const Action& action, State& state) {
    for (const auto& condition : action.conditions) {
        if (!state.hasProperty(condition)) {
            return false;
        }
    }
    return true;
}
void SpecificAction::Execute(State& state, const string& action) {
    // Nettoyage du state
    state.addProperty(action);
    if (state.hasProperty("Flee")) {
        state.removeProperty("Patrolling");
        state.removeProperty("Chase");
        state.removeProperty("Attack");
    }
    else if (state.hasProperty("Patrolling") || state.hasProperty("Attack")) {
        state.removeProperty("Chase");
        state.removeProperty("Attack");
    }
    else if (state.hasProperty("Chase")) {
        state.removeProperty("Patrolling");
        state.removeProperty("Attack");
    }
}

void SpecificAction::changeCost(int newCost) {
    cost = newCost;
}
int SpecificAction::GetCost() {
    return cost;
}

// --- PATROLLING ACTION --- //
PatrollingAction::PatrollingAction(int _cost, const vector<string>& _conditions, const vector<string>& _effects, const string& _id) {
    cost = _cost;
    conditions = _conditions;
    effects = _effects;
    id = _id;
}

// --- CHASE ACTION --- //
ChaseAction::ChaseAction(int _cost, const vector<string>& _conditions, const vector<string>& _effects, const string& _id) {
    cost = _cost;
    conditions = _conditions;
    effects = _effects;
    id = _id;
}

// --- SEARCH PLAYER ACTION --- //
AttackAction::AttackAction(int _cost, const vector<string>& _conditions, const vector<string>& _effects, const string& _id) {
    cost = _cost;
    conditions = _conditions;
    effects = _effects;
    id = _id;
}

// --- FLEE ACTION --- //
FleeAction::FleeAction(int _cost, const vector<string>& _conditions, const vector<string>& _effects, const string& _id) {
    cost = _cost;
    conditions = _conditions;
    effects = _effects;
    id = _id;
}