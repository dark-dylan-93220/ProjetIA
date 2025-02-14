#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity.hpp"

class Enemy : public Entity {
public:
    sf::Vector2f playerPos;
    static constexpr float SPEED = 100.0f;
    Enemy(float x, float y);
    void update(float deltaTime, Grid& grid) override;

private:
    sf::Vector2i position;
	bool needsRepath = false;
    int step = 0;
	void moveTowardsPlayer(sf::Vector2f& playerPos, Grid& grid, float deltaTime);
};

#endif // !ENEMY_HPP