#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

class Enemy : public Entity {
public:
    bool playerDetected; 
    bool playerInsight; 
    bool lowHP;
    static constexpr float SPEED = 100.0f;
    Enemy(float x, float y);
    void chase(float deltaTime, Grid& grid);
    void attack(float deltaTime, Grid& grid);
    void patrol(float deltaTime, Grid& grid);
    void flee(float deltaTime, Grid& grid);
};

#endif // ENEMY_HPP