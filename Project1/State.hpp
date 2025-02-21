#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class State {
public:
    vector<string> properties;
public:
    State(const vector<string>& baseProperties);

    // Getters
    bool hasProperty(const string& property) const;

    // Setters
    void addProperty(const string& property);
    void removeProperty(const string& property);
    
};