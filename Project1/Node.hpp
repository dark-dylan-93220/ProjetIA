#pragma once

#include <SFML/System/Vector2.hpp>
#include <cmath>

using namespace std;
using namespace sf;

struct Node {
    Vector2i position;
    float gCost, hCost, fCost;
    Node* parent;

    Node(Vector2i pos);
    void calculateCosts(Vector2i endPos, float newG);
    int calculateHeuristic(Vector2i endPos);
};