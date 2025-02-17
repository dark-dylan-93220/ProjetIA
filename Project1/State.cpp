#include "State.hpp"

State::State() {
	hasFood = false;
	hunger = 100;
}

// Getters
bool State::HasFood() const { 
	return hasFood; 
}
int State::GetHunger() const { 
	return hunger; 
}

// Setters
void State::SetFood(bool food) { 
	hasFood = food; 
}
void State::ReduceHunger() {
	hunger = std::max(0, hunger - 50); 
}
void State::SetHunger(int level) { 
	hunger = level; 
}