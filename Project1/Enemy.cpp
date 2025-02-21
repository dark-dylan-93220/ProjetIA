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
    Vector2i gridSize = { GRID_WIDTH - 1, GRID_HEIGHT - 1 };
}

Vector2i findFarthestTile(const Vector2i& origin, const Vector2i& gridSize);

Enemy::Enemy(float x, float y, int hp, const vector<Vector2f>& waypoints, const string& id) : Entity(x, y, Color::Red, hp)
{
    font.loadFromFile("assets/font.ttf");
    name.setFillColor(Color::Black);
    name.setOutlineColor(Color::White);
    name.setOutlineThickness(1);
    name.setFont(font);
    name.setString(id);
    name.setCharacterSize(20);
    name.setPosition(Vector2f(x + shape.getSize().x / 2 - name.getLocalBounds().width / 2, y + 60));
    // Utilisé pour identifier un ennemi avec sa méthode de refléxion
    identifiant = id;
    // Waypoints : points servants de repères pour la patrouille de l'ennemi, dépendant de chaque ennemi.
    _waypoints = waypoints;
    // Booléens servant à identifier l'état d'un ennemi
    needsRepath = false;
    playerDetected = false;
    playerInsight = false;
    if (hp <= 10) {
        lowHP = true;
    }
    else {
        lowHP = false;
    }
    // Informations globales sur l'état d'un ennemi
    AStarMoveDelay = 0;
    detectionRadius = circle.getRadius();
    position = Vector2i{ static_cast<int>(x), static_cast<int>(y) };
    followPath = { vector<Vector2i>{}, vector<bool>{} };
    currentState = PATROL;
}

void Enemy::pathCalculation(Vector2f playerPos, Grid& grid, const float& deltaTime) {
    float distance = (float)sqrt(pow(playerPos.x - shape.getPosition().x, 2) + pow(playerPos.y - shape.getPosition().y, 2));

    if (distance < 1 || distance > detectionRadius) {
        if (identifiant == "FSM") { currentState = PATROL; }
        return;
    }

    playerDetected = true;
    playerInsight = false;

    if (followPath.first.empty() || needsRepath) {
        future<vector<Vector2i>> result;

        start = {
            static_cast<int>(shape.getPosition().x / CELL_SIZE),
            static_cast<int>(shape.getPosition().y / CELL_SIZE)
        };
        end = {
            static_cast<int>(playerPos.x / CELL_SIZE),
            static_cast<int>(playerPos.y / CELL_SIZE)
        };
        fleeEnd = findFarthestTile(start, gridSize);

        // Utilisation de async et future pour le multithreading, afin de recevoir une valeur de retour (pathfinding)
        // L'ennemi fuit
        if (lowHP) {
            result = async(launch::async, Pathfinding::findPath, ref(grid), ref(start), ref(fleeEnd));
            followPathSteps = result.get();
        }
        // L'ennemi chasse
        else {
            future<vector<Vector2i>> result = async(launch::async, Pathfinding::findPath, ref(grid), ref(start), ref(end));
            followPathSteps = result.get();
        }

        // Le chemin est pris en compte seulement s'il contient au moins 2 étapes
        if (followPathSteps.size() <= 1) return;

        // A partir d'ici, l'ennemi chasse ou s'enfuit
        followPath = { followPathSteps, vector<bool>(followPathSteps.size(), false) };

        step = 0;
        needsRepath = false;
    }

    if (!followPath.first.empty() && step < followPath.first.size()) {
        if (lowHP) {
            return;
        }
        if (distance < CELL_SIZE) {
            playerDetected = false;
            playerInsight = true;
            enemyAttackPlayer = true;
        }
    }
}

void Enemy::enemyFollowsPath(const float& deltaTime) {
    position = followPath.first[step] * CELL_SIZE;

    Vector2i shapePos = { (int)shape.getPosition().x, (int)shape.getPosition().y };

    if ((shapePos / CELL_SIZE) == followPath.first[step] && AStarMoveDelay >= 0.3f) { // Minimum 0.3s entre chaque étape
        followPath.second[step] = true;
        AStarMoveDelay = 0;
        step++;
        if (step == followPath.first.size()) {
            followPath = { vector<Vector2i>{}, vector<bool>{} };
            needsRepath = true;
            return;
        }
    }

    Vector2f direction = Vector2f{ (float)position.x, (float)position.y } - shape.getPosition();
    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f && AStarMoveDelay >= 0.6f) {
        step++;
        AStarMoveDelay = 0;
    }
    else {
        direction /= distance;
    }

    shape.move(direction * SPEED * deltaTime);
    circle.move(direction * SPEED * deltaTime);
    name.move(direction * SPEED * deltaTime);

    if (step >= followPath.first.size()) {
        needsRepath = true;
        step = 0;
    }
    
    AStarMoveDelay += deltaTime;
}

void Enemy::attack(const float& deltaTime) {
    cout << this << " attaque!" << endl;
}

void Enemy::patrol(const float& deltaTime) {
    static int currentWaypoint = 0;

    Vector2f target = _waypoints[currentWaypoint];
    Vector2f direction = target - shape.getPosition();

    float distance = sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance < 5.0f) {
        currentWaypoint = (currentWaypoint + 1) % 4;
    }
    else {
        direction /= distance;
    }

    //--------------------------------------------
    shape.move(direction * SPEED * deltaTime);
    circle.move(direction * SPEED * deltaTime);
    name.move(direction * SPEED * deltaTime);
}

void Enemy::update(const float& deltaTime, Grid& grid, vector<shared_ptr<Entity>> players) {
    // Mise à jour des booléens possible dans 'pathCalculation()'
    playerDetected = false;
    playerInsight = false;
    enemyAttackPlayer = false;
    if (health <= 10) { lowHP = true; }

    // Fonction qui utilise A* et choisis de la suite de l'état d'un ennemi, quelle que soit sa méthode de réfléxion.
    pathCalculation(players[0]->shape.getPosition(), grid, deltaTime);
}

void Enemy::FSMAndGOAPUpdate(const float& deltaTime, const bool& playerDetected, const bool& playerInsight, const bool& lowHP,  const Vector2f& playerPos, State& enemyGoal, State& enemyState) {
    if (identifiant == "FSM") {
        switch (currentState) {
        case PATROL:
            patrol(deltaTime);
            enemyAttackPlayer = false;
            if (lowHP) {
                currentState = FLEE;
            }
            else if (playerDetected && !lowHP) {
                currentState = CHASE;
            }
            break;

        case CHASE:
            enemyFollowsPath(deltaTime);
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
            if (lowHP) { currentState = FLEE; }
            break;

        case FLEE:
            enemyAttackPlayer = false;
            enemyFollowsPath(deltaTime);
            break;
        }
    }
    else if (identifiant == "GOAP") {
        if (lowHP) {
            enemyGoal.properties = { "Flee" };
        }
        else if (playerDetected && !playerInsight) {
            enemyGoal.properties = { "Chase" };
        }
        else if (playerDetected && playerInsight) {
            enemyGoal.properties = { "Attack" };
        }
        else {
            enemyGoal.properties = { "Patrolling", "Tout" };
        }
        enemyState.properties = enemyGoal.properties;
    }
}

Vector2i findFarthestTile(const Vector2i& origin, const Vector2i& gridSize) {
    // Liste des coins de la grille
    Vector2i corners[4] = {
        {0, 0},                          // Coin en haut à gauche
        {0, gridSize.y - 1},             // Coin en bas à gauche
        {gridSize.x - 1, 0},             // Coin en haut à droite
        {gridSize.x - 1, gridSize.y - 1} // Coin en bas à droite
    };

    // Trouver le coin le plus éloigné en utilisant la distance de Manhattan
    Vector2i farthestTile = corners[0];
    int maxDistance = 0;

    for (const auto& corner : corners) {
        int distance = abs(corner.x - origin.x) + abs(corner.y - origin.y);

        if (distance > maxDistance) {
            maxDistance = distance;
            farthestTile = corner;
        }
    }

    // Pour éviter les murs
    if (farthestTile == Vector2i{0, 0}) {
        farthestTile = Vector2i{ 1,1 };
    }
    else if (farthestTile == Vector2i{ 0, 13 }) {
        farthestTile = Vector2i{ 1, 13 };
    }

    return farthestTile;
}