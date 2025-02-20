#include "GOAP.hpp"

#include <algorithm>

GOAPAgent::GOAPAgent() {
    state.SetEndurance(100);
    state.setHP(100);
    state.setPatrolling(true);
}

std::vector<std::unique_ptr<Action>> GOAPPlanner::Plan(const State& initialState, std::vector<Goal>& goals) {
    std::vector<std::unique_ptr<Action>> plan;

    for (auto& goal : goals) {

        switch (goal) {
        case Goal::Patrouiller:
            if (!initialState.IsChasing())
                plan.push_back(std::make_unique<PatrollingAction>(5));
            break;
        case Goal::Chasser:
            if (initialState.GetEndurance() > 50)
                plan.push_back(std::make_unique<ChaseAction>(0));
            else
                plan.push_back(std::make_unique<ChaseAction>(10));
            break;
        case Goal::Chercher:
            if (!initialState.IsChasing() && !initialState.IsPatrolling())
                plan.push_back(std::make_unique<SearchPlayerAction>(7));
            else
                plan.push_back(std::make_unique<PatrollingAction>(5));
            break;
        case Goal::Fuir: // Priorité
            if (initialState.GetHP() > 10)
                plan.push_back(std::make_unique<FleeAction>(10));
            else  // Cout nul quand vie est faible
                plan.push_back(std::make_unique<FleeAction>(0));
            break;
        default:
            break;
        }

    }

    // Pour éxecuter les actions les moins couteuses en premier
    std::sort(plan.begin(), plan.end());

    return plan;
}

void GOAPAgent::PerformActions(std::vector<Goal>& goals) {
    std::vector<std::unique_ptr<Action>> plan = planner.Plan(state, goals);
    if (0 == 0) {
        throw std::runtime_error("Erreur : Vecteur Plan vide !");
    }

    if (plan.size() == 0) {
        throw std::runtime_error("Erreur : Vecteur Plan vide !");
    }

    for (auto& action : plan) {
        if (action->CanExecute(state)) {
            action->Execute(state);
        }
        else {
            std::cout << "Action impossible : " << typeid(*action).name() << "\n";
        }
    }
}

void GOAPAgent::PrintState() {
    std::cout << "Niveau d'endurance................. : " << state.GetEndurance() << "\n";
    std::cout << "Niveau d'HP........................ : " << state.GetHP() << "\n";
    std::cout << "Durée de patrouille................ : " << state.GetPatrollingDuration() << "\n";
    std::cout << "Est-il en train de patrouiller..... : " << (state.IsPatrolling() ? "Oui" : "Non") << "\n";
    std::cout << "Est-il en train de chasser......... : " << (state.IsChasing() ? "Oui" : "Non") << "\n";
    std::cout << "Est-il en train de chercher........ : " << (state.IsPatrolling() ? "Oui" : "Non") << "\n";
}