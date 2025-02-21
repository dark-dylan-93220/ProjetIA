// player.cpp
#include "Player.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include "Enemy.hpp"

Player::Player(float x, float y, int hp) : Entity(x, y, Color::Blue, hp), attackTimer(0.f), hp(hp) {}

void Player::update(const float& deltaTime, Grid& grid, vector<shared_ptr<Entity>> entities) {

    vector<shared_ptr<Enemy>> enemies =
    {
        dynamic_pointer_cast<Enemy>(entities[0]),
        dynamic_pointer_cast<Enemy>(entities[1]),
        dynamic_pointer_cast<Enemy>(entities[2]),
    };

    Vector2f movement(0.f, 0.f);
    if (Keyboard::isKeyPressed(Keyboard::Z)) movement.y -= SPEED * deltaTime;
    if (Keyboard::isKeyPressed(Keyboard::S)) movement.y += SPEED * deltaTime;
    if (Keyboard::isKeyPressed(Keyboard::Q)) movement.x -= SPEED * deltaTime;
    if (Keyboard::isKeyPressed(Keyboard::D)) movement.x += SPEED * deltaTime;

    Vector2f newPosition = shape.getPosition() + movement;
    FloatRect newBounds(newPosition, shape.getSize());

    // Vérifier les quatre coins du joueur
    auto isWalkable = [&](float x, float y) {
        int gridX = static_cast<int>(x / CELL_SIZE);
        int gridY = static_cast<int>(y / CELL_SIZE);
        return gridX >= 0 && gridX < GRID_WIDTH && gridY >= 0 && gridY < GRID_HEIGHT && grid.getCell(gridX, gridY).walkable;
    };

    if (isWalkable(newBounds.left - 2, newBounds.top - 2) &&
        isWalkable(newBounds.left + newBounds.width + 2, newBounds.top - 2) &&
        isWalkable(newBounds.left - 2, newBounds.top + newBounds.height + 2) &&
        isWalkable(newBounds.left + newBounds.width + 2, newBounds.top + newBounds.height + 2)) {
        shape.move(movement);
    }

    attackTimer += deltaTime;
    if (Mouse::isButtonPressed(Mouse::Left) && attackTimer >= ATTACK_COOLDOWN) {
        attack(enemies);
        attackTimer = 0.f;
        shape.setFillColor(Color::Blue);
        shieldActive = false;
    }
    else if (Mouse::isButtonPressed(Mouse::Right)) {
        shieldActive = true;
        shape.setFillColor(Color::Green);
    }
    else {
        shape.setFillColor(Color::Blue);
        shieldActive = false;
    }
}

void Player::attack(vector<shared_ptr<Enemy>> enemies) {
    for (auto& enemy : enemies) {
        float distance = sqrt(pow(enemy->shape.getPosition().x - shape.getPosition().x, 2) + pow(enemy->shape.getPosition().y - shape.getPosition().y, 2));
        if (enemy->isAlive() && distance <= CELL_SIZE) {
            enemy->takeDamage(DAMAGE);
            cout << "Enemy HP: " << enemy->health << endl;
        }
    }
    cout << "Player attacks" << endl;
}