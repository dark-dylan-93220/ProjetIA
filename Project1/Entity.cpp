#include "Entity.hpp"

Entity::Entity(float x, float y, Color color, int hp) : health(hp) {
    shape.setSize({ 40, 40 });
    shape.setPosition(x, y);
    shape.setFillColor(color);

    circle.setFillColor(Color(0, 239, 87, 153));
    circle.setOutlineColor(Color(0, 239, 87));
    circle.setOutlineThickness(1);
    circle.setRadius(150);
    circle.setPosition(x - 130, y - 130);
}

bool Entity::isAlive() const {
    return health > 0;
}

void Entity::takeDamage(int damage) {
    health -= damage;
}

bool Entity::getStatutAtk() const {
    return enemyAttackPlayer;
}