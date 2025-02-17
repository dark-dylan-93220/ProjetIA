#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

struct Node {
    sf::Vector2i position;
    float gCost, hCost, fCost;
    Node* parent;

    Node(sf::Vector2i pos);
    void calculateCosts(sf::Vector2i endPos, float newG);
    int calculateHeuristic(sf::Vector2i endPos);
};