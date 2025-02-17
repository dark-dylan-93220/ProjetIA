#include "Actions.hpp"
#include "State.hpp"

bool EatAction::CanExecute(const State& state) {
    return state.HasFood() && state.GetHunger() > 0;
}

void EatAction::Execute(State& state) {
    std::cout << "L'agent mange.\n";
    state.ReduceHunger();  // R�duit la faim apr�s avoir mang�
    state.SetFood(false);   // Apr�s avoir mang�, il n'y a plus de nourriture
}

bool SearchFoodAction::CanExecute(const State& state) {
    return !state.HasFood();  // Peut chercher de la nourriture si l'agent n'en a pas
}

void SearchFoodAction::Execute(State& state) {
    std::cout << "L'agent cherche de la nourriture.\n";
    state.SetFood(true);  // Trouve de la nourriture
}