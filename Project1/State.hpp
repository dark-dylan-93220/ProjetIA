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
        // HP
    int GetHP() const;
        // Patrolling
    bool IsPatrolling() const;
    float GetPatrollingDuration() const;
        // Searching
    bool IsSearching() const;
        // Chasing
    bool IsChasing() const;
    int GetEndurance() const;

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