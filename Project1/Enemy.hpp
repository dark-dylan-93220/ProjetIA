#pragma once

#include "Entity.hpp"

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

    sf::Vector2i position;
    
public:
    Enemy(float x, float y, int hp);

public:
    void update(float deltaTime, Grid& grid, std::vector<std::shared_ptr<Entity>> players) override;
	void moveTowardsPlayer(sf::Vector2f playerPos, Grid& grid, float deltaTime);
    void chase(float deltaTime, Grid& grid);
    void attack(float deltaTime, Grid& grid);
    void patrol(float deltaTime, Grid& grid);
    void flee(float deltaTime, Grid& grid);
};