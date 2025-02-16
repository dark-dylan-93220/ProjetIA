#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Grid.hpp"
#include "BT.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jeu SFML - IA Ennemis");
    window.setFramerateLimit(60);
    
    sf::Clock clock;
    sf::Event event;

    Player player(200, 400);
    std::vector<Enemy> enemies = { Enemy(100, 100), Enemy(700, 100) };
    
    bool playerDetected = false;
    bool playerInsight = false;
    bool lowHP = false;
    
    Grid grid;
    grid.loadFromFile("map.txt");

    for (auto& enemy : enemies) {
        BTNode::makeTree(enemy, playerDetected, playerInsight, lowHP);
    }
    
    //BTNode->this(grid); ----------------------------------------------------------------------------------
    
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    
        for (auto& enemy : enemies) 
        {
            enemy.playerPos = player.shape.getPosition();
            enemy.update(deltaTime, grid);
    
            if (enemy.playerDetected) playerDetected = true; 
            else                      playerDetected = false;
            if (enemy.playerInsight)  playerInsight = true;
            else                      playerInsight = false; 
            if (enemy.lowHP)          lowHP = true;
            else                      lowHP = false;
        }

        player.update(deltaTime, grid);
    
        window.clear();

        grid.draw(window);
        window.draw(player.shape);
        for (const auto& enemy : enemies) 
        {
            window.draw(enemy.shape);
        }

        window.display();
    }
    
    return 0;
}