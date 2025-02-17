#pragma once

#include <iostream>
#include <vector>
#include <string>

class State {
private:
    bool hasFood;
    int hunger;
public:
    State();

    bool HasFood() const;
    int GetHunger() const;

    void SetFood(bool food);
    void ReduceHunger();
    void SetHunger(int level);
};