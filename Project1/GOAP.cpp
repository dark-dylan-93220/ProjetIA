#include "GOAP.hpp"

GOAPAgent::GOAPAgent() {
    state.SetHunger(100);  // Initialement, l'agent a faim
}

std::vector<std::unique_ptr<Action>> GOAPPlanner::Plan(const State& initialState, Goal& goal) {
    std::vector<std::unique_ptr<Action>> plan;

    if (goal == Goal::Manger) {
        if (initialState.GetHunger() > 0 && !initialState.HasFood()) {
            plan.push_back(std::move(std::make_unique<SearchFoodAction>()));
            plan.push_back(std::move(std::make_unique<EatAction>()));
        }
        else if (initialState.HasFood()) {
            plan.push_back(std::move(std::make_unique<EatAction>()));
        }
    }

    return plan;
}

void GOAPAgent::PerformActions() {
    Goal goal = Goal::Manger;  // L'objectif de l'agent est de manger

    std::vector<std::unique_ptr<Action>> plan = planner.Plan(state, goal);

    for (auto& action : plan) {
        if (action->CanExecute(state)) {
            action->Execute(state);  // Exécute l'action
        }
        else {
            std::cout << "Action impossible : " << typeid(*action).name() << "\n";
        }
    }
}

void GOAPAgent::PrintState() {
    std::cout << "Niveau de faim : " << state.GetHunger() << "\n";
    std::cout << "Nourriture disponible : " << (state.HasFood() ? "Oui" : "Non") << "\n";
}