#pragma once

#include "Entity.hpp"
#include "State.hpp"

enum StateFSM { PATROL, CHASE, FLEE, ATTACK };

class Enemy : public Entity {
public:
    bool playerDetected; 
    bool playerInsight; 
    bool lowHP;
    float attackCooldown = 0.f;
    string identifiant;

    Vector2f playerPos;

private:
    bool needsRepath;
    int step = 0;
    static constexpr float SPEED = 100.0f;
    float detectionRadius;
    float AStarMoveDelay;
    pair<vector<Vector2i>, vector<bool>> followPath;
    vector<Vector2i> followPathSteps;
    vector<Vector2f> _waypoints;
    Vector2i start;
    Vector2i end;
    Vector2i fleeEnd;
    Vector2i position;
    StateFSM currentState;
    
public:
    Enemy(float x, float y, int hp, const vector<Vector2f>& waypoints, const string& id);

public:
    // A* Pathinding
    void update(const float& deltaTime, Grid& grid, vector<shared_ptr<Entity>> players) override;
	void pathCalculation(Vector2f playerPos, Grid& grid, const float& deltaTime);

    // Les 3 ennemis utilisent ces foncions communément
    void attack();
    void patrol(const float& deltaTime);
    void enemyFollowsPath(const float& deltaTime);

    // FSM (partagée avec GOAP)
    void FSMAndGOAPUpdate(const float& deltaTime, const bool& playerDetected, const bool& playerInsight, const bool& lowHP, const Vector2f& playerPos, State& currentState, State& enemyState);
};