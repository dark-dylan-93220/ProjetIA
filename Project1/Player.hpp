#pragma once

#include "Enemy.hpp"

class Player : public Entity {
public:
    static constexpr float SPEED = 200.0f;
    static constexpr int DAMAGE = 20;
    static constexpr float ATTACK_COOLDOWN = 0.5f;
    float attackTimer;
    int hp;

    Player(float x, float y, int hp);
    void attack(std::vector<std::shared_ptr<Enemy>> enemies);
    void update(const float& deltaTime, Grid& grid, std::vector<std::shared_ptr<Entity>> enemies) override;
};