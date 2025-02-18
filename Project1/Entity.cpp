#include "Entity.hpp"

Entity::Entity(float x, float y, sf::Color color, int hp) : health(hp) {
    shape.setSize({ 40, 40 });
    shape.setPosition(x, y);
    shape.setFillColor(color);

    circle.setFillColor(sf::Color::Transparent);
    circle.setOutlineColor(sf::Color::Blue);
    circle.setOutlineThickness(2);
    circle.setRadius(200);
    circle.setPosition(x - 200, y - 200);
}

bool Entity::isAlive() const {
    return health > 0;
}

void Entity::takeDamage(int damage) {
    health -= damage;
}

bool Entity::getStatutAtk() {
    return enemiAttackPlayer;
}