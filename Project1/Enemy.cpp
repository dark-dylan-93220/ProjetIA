#include "Enemy.hpp"
#include "Pathfinding_A_Star.hpp"

#include <cmath>
#include <vector>
#include <utility>
#include <iostream>
#include <thread>
#include <functional>
#include <future>

namespace {
    float second = 0;
    sf::Vector2i gridSize = { GRID_WIDTH - 1, GRID_HEIGHT - 1 };
    std::thread t;
}

sf::Vector2i findFarthestTile(const sf::Vector2i& origin, const sf::Vector2i& gridSize);

Enemy::Enemy(float x, float y, int hp, const std::vector<sf::Vector2f>& waypoints) : Entity(x, y, sf::Color::Red, hp)
{
    _waypoints = waypoints;
    needsRepath = false;
    lowHP = false;
    if (hp <= 10)
        lowHP = true;
    playerDetected = false;
    playerInsight = false;
    position = sf::Vector2i{ static_cast<int>(x), static_cast<int>(y) };
    followPath = { std::vector<sf::Vector2i>{}, std::vector<bool>{} };
    currentState = PATROL;
}

void Enemy::update(const float& deltaTime, Grid& grid, std::vector<std::shared_ptr<Entity>> players) {
    moveTowardsPlayer(players[0]->shape.getPosition(), grid, deltaTime);
}

void Enemy::chase(const float& deltaTime, Grid& grid) { // --------------------------------------------------
    std::cout << this << " : En chasse" << std::endl;
}
void Enemy::attack(const float& deltaTime, Grid& grid) {
    std::cout << this << " : En attaque" << std::endl;
}
void Enemy::patrol(const float& deltaTime, Grid& grid) {
    std::cout << this << " : En patrouille" << std::endl;
    FSMpatrol(deltaTime, *this);
}
void Enemy::flee(const float& deltaTime, Grid& grid) {
    std::cout << this << " : En fuite" << std::endl;
}

void Enemy::moveTowardsPlayer(sf::Vector2f playerPos, Grid& grid, const float& deltaTime) {
    float distance = (float)std::sqrt(std::pow(playerPos.x - shape.getPosition().x, 2) + std::pow(playerPos.y - shape.getPosition().y, 2));
    playerDetected = false;
    playerInsight = false;
    enemyAttackPlayer = false;

    if (health <= 10) {
        lowHP = true;
        shape.setFillColor(sf::Color::Green);
    }
    else {
        shape.setFillColor(sf::Color::Red);
    }
    //std::cout << this << " Low HP : " << lowHP << std::endl;

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

        std::thread pathfindingThread;
        

        if (lowHP) {
            std::future<std::vector<sf::Vector2i>> result = std::async(std::launch::async, Pathfinding::findPath, std::ref(grid), std::ref(start), std::ref(fleeEnd));
            followPathSteps = result.get();
        }
        else {
            std::future<std::vector<sf::Vector2i>> result = std::async(std::launch::async, Pathfinding::findPath, std::ref(grid), std::ref(start), std::ref(end));
            followPathSteps = result.get();
        }

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

        sf::Vector2f direction = sf::Vector2f{ (float)position.x, (float)position.y } - shape.getPosition();

        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

        if (distance < 5.0f) {
            step++;
        }
        else {
            direction /= distance;
        }

        shape.move(direction * SPEED * deltaTime);
        circle.move(direction * SPEED * deltaTime);

        // 3 étapes par seconde minimum.
        if (second >= 0.6f) {
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

void Enemy::FSMupdate(const float& deltaTime, const bool& playerDetected, const bool& playerInsight, const bool& lowHP,  const sf::Vector2f& playerPos) {

    switch (currentState) {
    case PATROL:
        FSMpatrol(deltaTime, *this);

        std::cout << "patrolFSM" << std::endl;
        enemyAttackPlayer = false;

        if (lowHP) { 
            currentState = FLEE; 
        }
        if (playerDetected && !lowHP) {
            currentState = CHASE;
        }
        break;

    case CHASE:
        FSMchase(deltaTime, playerPos);

        std::cout << "chaseFSM" << std::endl;
        enemyAttackPlayer = false;

        if (lowHP) { 
            currentState = FLEE; 
        }

        if (!playerDetected) {
            if (playerInsight) { 
                currentState = ATTACK; 
            }
            else { 
                currentState = PATROL; 
            }
        }
        break;

    case ATTACK:
        enemyAttackPlayer = true;
        std::cout << "attackFSM" << std::endl;

        if (lowHP) { 
            currentState = FLEE; 
        }
        break;

    case FLEE:
        enemyAttackPlayer = false;
        std::cout << "fleeFSM" << std::endl;
        break;
    }
}

void Enemy::FSMpatrol(const float& deltaTime, Enemy& enemy) {
    static int currentWaypoint = 0;

    sf::Vector2f target = _waypoints[currentWaypoint];
    sf::Vector2f direction = target - shape.getPosition();

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f) {
        currentWaypoint = (currentWaypoint + 1) % 4;
    }
    else {
        direction /= distance;
    }

    std::cout << this << " patrouille" << std::endl;

    enemy.shape.move(direction * SPEED * deltaTime);
    enemy.circle.move(direction * SPEED * deltaTime);
}

void Enemy::FSMchase(const float& deltaTime, const sf::Vector2f& playerPos) {
    sf::Vector2f direction = playerPos - shape.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0) {
        direction /= distance;
    }

    shape.move(direction * SPEED * deltaTime);
    circle.move(direction * SPEED * deltaTime);
}

void Enemy::FSMFlee(const float& deltaTime, const sf::Vector2f& playerPos) {
    sf::Vector2f direction = playerPos - shape.getPosition();
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance > 0) {
        direction /= distance;
    }

    shape.move(direction * SPEED * deltaTime);
    circle.move(direction * SPEED * deltaTime);
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
        int distance = std::abs(corner.x - origin.x) + std::abs(corner.y - origin.y);

        if (distance > maxDistance) {
            maxDistance = distance;
            farthestTile = corner;
        }
    }

    // Pour éviter les murs
    if (farthestTile == sf::Vector2i{0, 0}) {
        farthestTile = sf::Vector2i{ 1,1 };
    }
    else if (farthestTile == sf::Vector2i{ 0, 13 }) {
        farthestTile = sf::Vector2i{ 1, 13 };
    }

    return farthestTile;
}