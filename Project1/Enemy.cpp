#include "Enemy.hpp"
#include "Pathfinding_A_Star.hpp"

#include <cmath>
#include <vector>
#include <utility>
#include <iostream>

namespace {
    float second = 0;
    sf::Vector2i gridSize = { GRID_WIDTH - 1, GRID_HEIGHT - 1 };
}

sf::Vector2i findFarthestTile(const sf::Vector2i& origin, const sf::Vector2i& gridSize);

Enemy::Enemy(float x, float y, int hp) : Entity(x, y, sf::Color::Red, hp)
{
    needsRepath = false;
    lowHP = false;
    playerDetected = false;
    playerInsight = false;
    position = sf::Vector2i{ static_cast<int>(x), static_cast<int>(y) };
    followPath = { std::vector<sf::Vector2i>{}, std::vector<bool>{} };
}

void Enemy::update(float deltaTime, Grid& grid, std::vector<std::shared_ptr<Entity>> players) {
    moveTowardsPlayer(players[0]->shape.getPosition(), grid, deltaTime);
}

void Enemy::chase(float deltaTime, Grid& grid) { // --------------------------------------------------
    std::cout << "En chasse" << std::endl;
}
void Enemy::attack(float deltaTime, Grid& grid) {
    std::cout << "En attaque" << std::endl;
}
void Enemy::patrol(float deltaTime, Grid& grid) {
    std::cout << "En patrouille" << std::endl;
}
void Enemy::flee(float deltaTime, Grid& grid) {
    std::cout << "En fuite" << std::endl;
}

void Enemy::moveTowardsPlayer(sf::Vector2f playerPos, Grid& grid, float deltaTime) {
    float distance = (float)std::sqrt(std::pow(playerPos.x - shape.getPosition().x, 2) + std::pow(playerPos.y - shape.getPosition().y, 2));
    playerDetected = false;
    playerInsight = false;
    enemyAttackPlayer = false;

    if (health <= 10) {
        lowHP = true;
    }
    std::cout << this << " Low HP : " << lowHP << std::endl;

    if (distance < 1 || distance > 200) return;

    playerDetected = true;
    playerInsight = false;

    if (distance < CELL_SIZE) {
        playerDetected = false;
        playerInsight = true; 
        enemyAttackPlayer = true;
    }

    if (followPath.first.empty() || needsRepath) {
        start = { 
            static_cast<int>(shape.getPosition().x / CELL_SIZE), 
            static_cast<int>(shape.getPosition().y / CELL_SIZE) 
        };
        end = { 
            static_cast<int>(playerPos.x / CELL_SIZE),             
            static_cast<int>(playerPos.y / CELL_SIZE) 
        };
        fleeEnd = findFarthestTile(start, gridSize);

        if (lowHP) {
            followPathSteps = Pathfinding::findPath(grid, start, fleeEnd);
        }
        else
            followPathSteps = Pathfinding::findPath(grid, start, end);

        // Le chemin est pris en compte seulement s'il contient au moins 2 étapes
        if (followPathSteps.size() <= 1) return;

        std::cout << "Chemin calcule : ";
        for (const auto& step : followPathSteps) {
            std::cout << "(" << step.x << ", " << step.y << ") ";
        }
        std::cout << std::endl;

        // A partir d'ici, l'ennemi chasse
        followPath = { followPathSteps, std::vector<bool>(followPathSteps.size(), false) };

        // Réinitialisation des valeurs clés du chemin à 0
        step = 0;
        needsRepath = false;
    }

    if (!followPath.first.empty() && step < followPath.first.size()) {
        position = followPath.first[step] * CELL_SIZE;

        sf::Vector2i shapePos = { (int)shape.getPosition().x, (int)shape.getPosition().y };

        if ((shapePos / CELL_SIZE) == followPath.first[step]) {
            followPath.second[step] = true;
            second = 0;
            step++;
            if (step == followPath.first.size()) {
                followPath = { std::vector<sf::Vector2i>{}, std::vector<bool>{} };
                needsRepath = true;
                return;
            }
        }

        float dx = position.x - shape.getPosition().x;
        float dy = position.y - shape.getPosition().y;
        float angle = std::atan2(dy, dx);

        shape.move(SPEED * std::cos(angle) * deltaTime, SPEED * std::sin(angle) * deltaTime);
        circle.move(SPEED * std::cos(angle) * deltaTime, SPEED * std::sin(angle) * deltaTime);

        // 3 étapes par seconde minimum.
        if (second >= 0.8f) {
            step++;
            second = 0;
        }

        if (step >= followPath.first.size()) {
            needsRepath = true;
            step = 0;
        }
    }
    second += deltaTime;
}

sf::Vector2i findFarthestTile(const sf::Vector2i& origin, const sf::Vector2i& gridSize) {
    // Liste des coins de la grille
    sf::Vector2i corners[4] = {
        {0, 0},                          // Coin en haut à gauche
        {0, gridSize.y - 1},             // Coin en bas à gauche
        {gridSize.x - 1, 0},             // Coin en haut à droite
        {gridSize.x - 1, gridSize.y - 1} // Coin en bas à droite
    };

    // Trouver le coin le plus éloigné en utilisant la distance de Manhattan
    sf::Vector2i farthestTile = corners[0];
    int maxDistance = 0;

    for (const auto& corner : corners) {
        int distance = std::abs(corner.x - origin.x) + std::abs(corner.y - origin.y); // Distance de Manhattan

        if (distance > maxDistance) {
            maxDistance = distance;
            farthestTile = corner;
        }
    }

    if (farthestTile == sf::Vector2i{0, 0}) {
        farthestTile = sf::Vector2i{ 1,1 };
    }
    else if (farthestTile == sf::Vector2i{ 0, 13 }) {
        farthestTile = sf::Vector2i{ 1, 13 };
    }

    return farthestTile;
}