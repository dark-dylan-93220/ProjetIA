#include "Actions.hpp"
#include "State.hpp"


// --- PATROLLING ACTION --- //
PatrollingAction::PatrollingAction(int _cost) {
    cost = _cost;
}
bool PatrollingAction::CanExecute(const State& state) {
    return !state.IsChasing() || state.IsSearching();
}
void PatrollingAction::Execute(State& state) {
    std::cout << "L'agent patrouille.\n";
    state.SetPatrollingDuration(10.f);
    state.setSearching(false);
    state.setPatrolling(true);
}
void PatrollingAction::changeCost(int newCost) {
    cost = newCost;
}

// --- CHASE ACTION --- //
ChaseAction::ChaseAction(int _cost) {
    cost = _cost;
}
bool ChaseAction::CanExecute(const State& state) {
    return state.GetEndurance() > 10;
}
void ChaseAction::Execute(State& state) {
    std::cout << "L'agent chasse le joueur.\n";
    state.SetChasing(true);
}
void ChaseAction::changeCost(int newCost) {
    cost = newCost;
}

// --- SEARCH PLAYER ACTION --- //
SearchPlayerAction::SearchPlayerAction(int _cost) {
    cost = _cost;
}
bool SearchPlayerAction::CanExecute(const State& state) {
    return state.IsChasing();  // Peut chercher le joueur seulement s'il chasse
}
void SearchPlayerAction::Execute(State& state) {
    std::cout << "L'agent cherche le joueur.\n";
    state.SetChasing(false);
    state.setSearching(true);
}
void SearchPlayerAction::changeCost(int newCost) {
    cost = newCost;
}

// FLEE ACTION
FleeAction::FleeAction(int _cost) {
    cost = _cost;
}
bool FleeAction::CanExecute(const State& state) {
    return state.GetHP() <= 10;
}
void FleeAction::Execute(State& state) {
    std::cout << "L'agent fuit.\n";
    state.SetChasing(false);
    state.setSearching(false);
    state.setPatrolling(false);
}
void FleeAction::changeCost(int newCost) {
    cost = newCost;
}