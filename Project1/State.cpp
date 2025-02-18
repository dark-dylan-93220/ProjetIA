#include "State.hpp"

State::State() {
	isPatrolling = true;
	isSearching = false;
	isChasing = false;
	endurance = 100;  // 100 = full, 0 = none
	hp = 100;
	patrollingDuration = 0.f;
}

// Getters
bool State::IsChasing() const { 
	return isChasing; 
}
bool State::IsPatrolling() const {
	return isPatrolling;
}
bool State::IsSearching() const {
	return isSearching;
}
int State::GetEndurance() const { 
	return endurance; 
}
int State::GetHP() const {
	return hp;
}
float State::GetPatrollingDuration() const {
	return patrollingDuration;
}

// Setters
void State::SetChasing(bool chasing) { 
	isChasing = chasing; 
}
void State::setPatrolling(bool patrolling) {
	isPatrolling = patrolling;
}
void State::setSearching(bool searching) {
	isSearching = searching;
}
void State::ReduceEndurance(int malus) {
	endurance = std::max(0, endurance - malus); 
}
void State::SetEndurance(int level) { 
	endurance = level; 
}
void State::setHP(int level) {
	hp = level;
}
void State::SetPatrollingDuration(float level) {
	patrollingDuration = level;
}