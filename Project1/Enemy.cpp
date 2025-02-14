#include "Enemy.hpp"
#include "Pathfinding_A_Star.hpp"
#include <cmath>
#include <iostream>

Enemy::Enemy(float x, float y) : Entity(x, y, sf::Color::Red) 
{
	position = sf::Vector2i{ static_cast<int>(x), static_cast<int>(y) };
}

void Enemy::update(float deltaTime, Grid& grid) {
	moveTowardsPlayer(playerPos, grid, deltaTime);
	shape.setPosition(position.x, position.y);
}

void Enemy::moveTowardsPlayer(sf::Vector2f& playerPos, Grid& grid, float deltaTime) {
	// Calculer la distance entre l'ennemi et le joueur
	float distance = std::sqrt(std::pow(playerPos.x - shape.getPosition().x, 2) + std::pow(playerPos.y - shape.getPosition().y, 2));
	std::cout << distance << std::endl;
	// Si la distance est inférieure à 1 ou supérieure à 100, continuer la patrouille
	if (distance < 1 || distance > 200) return;

	// Calculer le chemin entre l'ennemi et le joueur

	sf::Vector2i start = { static_cast<int>(shape.getPosition().x), static_cast<int>(shape.getPosition().y) };
	sf::Vector2i end = { static_cast<int>(playerPos.x), static_cast<int>(playerPos.y) };
	std::vector<sf::Vector2i> path = Pathfinding::findPath(grid, start, end);

	std::cout << "Start : " << start.x << " " << start.y << std::endl;
	std::cout << "End : " << end.x << " " << end.y << std::endl;
	std::cout << "Path : ";
	for (auto& p : path)
		std::cout << p.x << " " << p.y;
	std::cout << std::endl;

	// Déplacer l'ennemi vers le joueur

	if (!path.empty() && grid.getCell(path[step].y, path[step].x).walkable) {
		needsRepath = true;
	}
	else {
		needsRepath = false;
	}

	if (needsRepath || path.empty()) {
		path = Pathfinding::findPath(grid, start, end);
		step = 0;
		needsRepath = false;
	}

	if (!path.empty() && step < path.size()) {
		position = path[step];
		if (step != path.size() - 1)
			step++;
	}
}