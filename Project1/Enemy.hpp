#pragma once

#include "Entity.hpp"

enum StateFSM { PATROL, CHASE, FLEE, ATTACK };

class Enemy : public Entity {
public:
    bool playerDetected; 
    bool playerInsight; 
    bool lowHP;

    sf::Vector2f playerPos;

private:
    bool needsRepath;
    int step = 0;
    static constexpr float SPEED = 100.0f;
    typedef std::pair<std::vector<sf::Vector2i>, std::vector<bool>> followPath_t;
    followPath_t followPath;
    std::vector<sf::Vector2i> followPathSteps;
    sf::Vector2i start;
    sf::Vector2i end;
    sf::Vector2i fleeEnd;
    sf::Vector2i position;
    StateFSM currentState;
    
public:
    Enemy(float x, float y, int hp);

public:
    // A* Pathinding
    void update(float deltaTime, Grid& grid, std::vector<std::shared_ptr<Entity>> players) override;
	void moveTowardsPlayer(sf::Vector2f playerPos, Grid& grid, float deltaTime);

    // Behavior tree & GOAP states
    void chase(float deltaTime, Grid& grid);
    void attack(float deltaTime, Grid& grid);
    void patrol(float deltaTime, Grid& grid);
    void flee(float deltaTime, Grid& grid);

    // FSM
    void FSMupdate(const bool& playerDetected, const bool& playerInsight, const bool& lowHP, const sf::Vector2f& playerPos);
    void FSMpatrol();
    void FSMchase(const sf::Vector2f& playerPos);
    void FSMFlee(const sf::Vector2f& playerPos);
};