#include "GOAP.hpp"

#include <algorithm>
#include <unordered_set>

GOAPAgent::GOAPAgent() {
    state.properties = { "Patrolling", "Tout" }; // Par défaut, l'agent patrouille
}

vector<SpecificAction> GOAPPlanner::Plan(const State& initialState, State& goalState, vector<SpecificAction>& actions) {
    vector<SpecificAction> plan;
    State currentState = initialState;

    while (true) {
        unordered_set<string> currentProperties(currentState.properties.begin(), currentState.properties.end());
        bool allGoalsMet = std::all_of(goalState.properties.begin(), goalState.properties.end(), [&](const std::string& prop) 
            {
                return std::find(currentState.properties.begin(), currentState.properties.end(), prop) != currentState.properties.end();
            }
        );

        if (currentState.properties == goalState.properties) {
            plan.clear();
            if (goalState.hasProperty("Patrolling")) {
                plan.push_back(PatrollingAction(5, { "Tout" }, { "Patrolling", "Tout" }, "Patrolling"));
            }
            else if (goalState.hasProperty("Chase")) {
                plan.push_back(ChaseAction(15, { "Patrolling" }, { "Chase", "Tout" }, "Chase"));
            }
            else if (goalState.hasProperty("Flee")) {
                plan.push_back(FleeAction(0, { "Tout" }, { "Flee", "Tout" }, "Flee"));
            }
            else if (goalState.hasProperty("Attack")) {
                plan.push_back(AttackAction(5, { "Chase" }, { "Attack", "Tout" }, "Attack"));
            }
        }

        if (allGoalsMet) {
            break;
        }

        int minCost = INT_MAX;
        SpecificAction* bestAction = nullptr;

        for (auto& action : actions) {
            if (action.CanExecute(action, currentState) && action.GetCost() < minCost) {
                minCost = action.GetCost();
                bestAction = &action;
            }
        }

        if (bestAction != nullptr) {
            bestAction->Execute(currentState, bestAction->id);
        }
        else {
            cout << "Aucune action executable trouvee." << endl;
            return {};
        }

        if (minCost == INT_MAX) {
            cout << "Aucun plan trouve." << endl;
            return {};
        }

        SpecificAction::applyAction(*bestAction, currentState);
        plan.push_back(*bestAction);
        
    }

    return plan;
}


void GOAPAgent::PerformActions(State& goalState, vector<SpecificAction>& actions, Enemy& enemy, const float& deltaTime) {

    vector<SpecificAction> plan = planner.Plan(state, goalState, actions);


    // Méthode runtime_error utilisée pour le try-catch
    if (plan.size() == 0) {
        throw runtime_error("Erreur : Vecteur Plan vide !");
    }

    if (!plan.empty()) {
        cout << "Plan trouve : ";
        int count = 0;
        for (const auto& action : plan) {
            if (count == plan.size() - 1) {
                cout << action.id << " (cout: " << action.cost << ")" << endl;
            }
            else {
                cout << action.id << " (cout: " << action.cost << ") -> ";
            }
            count++;
        }
    }

    for (auto& action : plan) {
        //action.Execute(state, action.id);
        if (action.id == "Patrolling") {
            enemy.patrol(deltaTime);
        }
        else if (action.id == "Chase" || action.id == "Flee") {
            enemy.enemyFollowsPath(deltaTime);
        }
        else if (action.id == "Attack" && enemy.attackCooldown >= 1.f) {
            enemy.enemyAttackPlayer = true;
        }
        else {
            enemy.enemyAttackPlayer = false;
        }
    }
}

void GOAPAgent::PrintState(State& currentState) {
    cout << "-----------------------------------------" << endl;
    cout << "Etat de l'ennemi GOAP :";
    for (auto& property : state.properties) {
        if (property == "Tout") continue;
        cout << " " << property << endl;
    }
    cout << "-----------------------------------------" << endl;
}