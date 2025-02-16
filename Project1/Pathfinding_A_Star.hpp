#pragma once

#include "Grid.hpp"
#include "Node.hpp"
#include <vector>
#include <iostream>

class Pathfinding {
public:
    static std::vector<sf::Vector2i> findPath(Grid& grid, sf::Vector2i start, sf::Vector2i end);
};