#include "State.hpp"

State::State(const vector<string>& baseProperties) {
	properties = baseProperties;
}

// Getters
bool State::hasProperty(const string& property) const {
	return find(properties.begin(), properties.end(), property) != properties.end();
}


// Setters
void State::addProperty(const string& property) {
	if (!hasProperty(property)) {
		properties.push_back(property);
	}
}
void State::removeProperty(const string& property) {
	auto it = find(properties.begin(), properties.end(), property);
	if (it != properties.end()) {
		properties.erase(it);
	}
}