#include "Enemy.hpp"
#include <cmath>
#include <iostream>

Enemy::Enemy(float x, float y) : Entity(x, y, sf::Color::Red) {}

void Enemy::chase(float deltaTime, Grid& grid) {
	std::cout << "chase" << std::endl;
}
void Enemy::attack(float deltaTime, Grid& grid) {
	std::cout << "attack" << std::endl;
}
void Enemy::patrol(float deltaTime, Grid& grid) {
	std::cout << "patrol" << std::endl;
}
void Enemy::flee(float deltaTime, Grid& grid) {
	std::cout << "flee" << std::endl;
}

