#pragma once

#include <SFML/System/Vector2.hpp>

struct Node {
    sf::Vector2i position;
    float gCost, hCost, fCost;
    Node* parent;

    Node(sf::Vector2i pos);
    void calculateCosts(Node* end, float newG);
};