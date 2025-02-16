#include "Enemy.hpp"
#include "Pathfinding_A_Star.hpp"

#include <cmath>
#include <vector>
#include <utility>
#include <iostream>

namespace {
    float second = 0;
    typedef std::pair<std::vector<sf::Vector2i>, std::vector<bool>> followPath_t;
    followPath_t followPath = { std::vector<sf::Vector2i>{}, std::vector<bool>{} };
    sf::Vector2i start;
    sf::Vector2i end;
}

Enemy::Enemy(float x, float y) : Entity(x, y, sf::Color::Red)
{
    position = sf::Vector2i{ static_cast<int>(x), static_cast<int>(y) };
}

void Enemy::update(float deltaTime, Grid& grid) {
    moveTowardsPlayer(playerPos, grid, deltaTime);
}

void Enemy::moveTowardsPlayer(sf::Vector2f& playerPos, Grid& grid, float deltaTime) {
    float distance = std::sqrt(std::pow(playerPos.x - shape.getPosition().x, 2) + std::pow(playerPos.y - shape.getPosition().y, 2));

    if (distance < 1 || distance > 200) return;

    if (followPath.first.empty() || needsRepath) {
        start = { static_cast<int>(shape.getPosition().x / CELL_SIZE), static_cast<int>(shape.getPosition().y / CELL_SIZE) };
        end = { static_cast<int>(playerPos.x / CELL_SIZE),             static_cast<int>(playerPos.y / CELL_SIZE) };
        std::vector<sf::Vector2i> followPathSteps = Pathfinding::findPath(grid, start, end);

        if (followPathSteps.size() <= 1) return;
        std::cout << "Chemin calcule : ";
        for (const auto& step : followPathSteps) {
            std::cout << "(" << step.x << ", " << step.y << ") ";
        }
        std::cout << std::endl;

        followPath = { followPathSteps, std::vector<bool>(followPathSteps.size(), false) };
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

        // 1 �tape par 60 boucles
        if (second >= 1) {
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