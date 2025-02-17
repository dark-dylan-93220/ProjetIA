#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Enemy.hpp"
#include "GOAP.hpp"
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

    auto root1 = std::make_unique<SelectorNode>();
    auto root2 = std::make_unique<SelectorNode>();
    auto root3 = std::make_unique<SelectorNode>();
    auto root4 = std::make_unique<SelectorNode>();

    auto sequenceEnemyOne1 = std::make_unique<SequenceNode>();
    auto sequenceEnemyOne2 = std::make_unique<SequenceNode>();
    auto sequenceEnemyOne3 = std::make_unique<SequenceNode>();
    auto sequenceEnemyOne4 = std::make_unique<SequenceNode>();

    auto sequenceEnemyTwo1 = std::make_unique<SequenceNode>();
    auto sequenceEnemyTwo2 = std::make_unique<SequenceNode>();
    auto sequenceEnemyTwo3 = std::make_unique<SequenceNode>();
    auto sequenceEnemyTwo4 = std::make_unique<SequenceNode>();
    Blackboard bb;

    for (int i = 0; i < 2; ++i) {
        if (i == 0)
            InheritFromEveryone::makeTree(root1, root2, sequenceEnemyOne1, sequenceEnemyOne2, sequenceEnemyOne3, sequenceEnemyOne4, bb, enemies[i], playerDetected, playerInsight, lowHP, grid);
        else
            InheritFromEveryone::makeTree(root3, root4, sequenceEnemyTwo1, sequenceEnemyTwo2, sequenceEnemyTwo3, sequenceEnemyTwo4, bb, enemies[i], playerDetected, playerInsight, lowHP, grid);
    }

    GOAPAgent agent;

    std::cout << "Etat initial de l'agent:\n";
    agent.PrintState();

    std::cout << "\nL'agent commence ses actions...\n";
    agent.PerformActions();  // L'agent va chercher de la nourriture, puis manger

    std::cout << "\nEtat de l'agent après avoir effectue les actions:\n";
    agent.PrintState();
    
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
            // Mise à jour du behavior tree et exécution de ce dernier
            InheritFromEveryone::executeTree(root1, bb, playerDetected, playerInsight, lowHP);
            // Mise à jour de la position du joueur pour les ennemis
            enemy.playerPos = player.shape.getPosition();
            enemy.update(deltaTime, grid);
            /*std::cout << enemy.playerInsight << std::endl;
            std::cout << enemy.playerDetected << std::endl;
            std::cout << enemy.lowHP << std::endl;*/

            // Mise à jour des valeurs booléennes des ennemis
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