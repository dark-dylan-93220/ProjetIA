#pragma once

#include <iostream>
#include <vector>
#include <string>

class State {
private:
    bool isChasing;
    bool isPatrolling;
    bool isSearching;
    int endurance;
    int hp;
    float patrollingDuration;
public:
    State();

    // Getters
    bool IsChasing() const;
    bool IsPatrolling() const;
    bool IsSearching() const;
    int GetEndurance() const;
    int GetHP() const;
    float GetPatrollingDuration() const;

    // Setters
        // HP
    void setHP(int level);
        // Patrolling
    void setPatrolling(bool patrolling);
    void SetPatrollingDuration(float level);
        // Searching
    void setSearching(bool searching);
        // Chasing
    void SetChasing(bool chasing);
    void ReduceEndurance(int malus);
    void SetEndurance(int level);
    
};