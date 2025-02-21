#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "Grid.hpp"

using namespace std;
using namespace sf;

class Entity {
public:
    Entity(float x, float y, Color color, int hp);
    RectangleShape shape;
    Vector2f velocity;
    CircleShape circle;
    int health;
    bool enemyAttackPlayer = false;
    virtual void update(const float& deltaTime, Grid& grid, vector<shared_ptr<Entity>> neededEntities) = 0;

public:
    bool isAlive() const;
    void takeDamage(int damage);
    bool getStatutAtk() const;
};

#endif // ENTITY_HPP