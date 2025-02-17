#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>
#include "Grid.hpp"

class Entity {
public:
    Entity(float x, float y, sf::Color color, int hp);
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    int health;
    virtual void update(float deltaTime, Grid& grid, std::vector<std::shared_ptr<Entity>> neededEntities) = 0;

public:
    bool isAlive() const;
    void takeDamage(int damage);
};

#endif // ENTITY_HPP