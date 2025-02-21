#pragma once

#include "Enemy.hpp"

class Player : public Entity {
public:
    static constexpr float SPEED = 200.0f;
    static constexpr int DAMAGE = 20;
    static constexpr float ATTACK_COOLDOWN = 0.5f;
    float attackTimer;
    int hp;
    bool shieldActive = false;

    Player(float x, float y, int hp);
    void attack(vector<shared_ptr<Enemy>> enemies);
    void update(const float& deltaTime, Grid& grid, vector<shared_ptr<Entity>> enemies) override;
};