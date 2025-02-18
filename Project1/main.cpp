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

    std::vector<std::shared_ptr<Entity>> players = { std::make_shared<Player>(400, 400, 10) };
    std::vector<std::shared_ptr<Entity>> enemies = { std::make_shared<Enemy>(100, 100, 1), std::make_shared<Enemy>(700, 100, 100) };
    //std::vector<std::shared_ptr<Enemy>> trueEnemies = { std::dynamic_pointer_cast<Enemy>(enemies[0]), std::dynamic_pointer_cast<Enemy>(enemies[1]) };
    
    //bool playerDetected = false;
    //bool playerInsight = false;
    //bool lowHP = false;
    
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
            InheritFromEveryone::makeTree(root1, root2, sequenceEnemyOne1, sequenceEnemyOne2, sequenceEnemyOne3, sequenceEnemyOne4, bb, enemies[i], std::dynamic_pointer_cast<Enemy>(enemies[i])->playerDetected, std::dynamic_pointer_cast<Enemy>(enemies[i])->playerInsight, std::dynamic_pointer_cast<Enemy>(enemies[i])->lowHP, grid);
        else
            InheritFromEveryone::makeTree(root3, root4, sequenceEnemyTwo1, sequenceEnemyTwo2, sequenceEnemyTwo3, sequenceEnemyTwo4, bb, enemies[i], std::dynamic_pointer_cast<Enemy>(enemies[i])->playerDetected, std::dynamic_pointer_cast<Enemy>(enemies[i])->playerInsight, std::dynamic_pointer_cast<Enemy>(enemies[i])->lowHP, grid);
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
            std::cout << "Etat enemi " << enemy.get() << " : ";
            //std::cout << std::dynamic_pointer_cast<Enemy>(enemy)->playerDetected << std::endl;
            if (enemy->isAlive()) {
                // Mise à jour du behavior tree et exécution de ce dernier
                if(std::dynamic_pointer_cast<Enemy>(enemy) == enemy)
                    InheritFromEveryone::executeTree(root1, bb, std::dynamic_pointer_cast<Enemy>(enemy)->playerDetected, std::dynamic_pointer_cast<Enemy>(enemy)->playerInsight, std::dynamic_pointer_cast<Enemy>(enemy)->lowHP);
                // Mise à jour de la position du joueur pour les ennemis
                enemy->update(deltaTime, grid, players);
            }
        }

        //for (auto& enemy : trueEnemies) {
        //    if (enemy->isAlive()) {
        //        enemy->playerPos = player.shape.getPosition();
        //        // Mise à jour des valeurs booléennes des ennemis
        //        if (enemy->playerDetected) playerDetected = true;
        //        else                       playerDetected = false;
        //        if (enemy->playerInsight)  playerInsight = true;
        //        else                       playerInsight = false;
        //        if (enemy->lowHP)          lowHP = true;
        //        else                       lowHP = false;
        //    }
        //}

        players[0]->update(deltaTime, grid, enemies);
        if (players[0]->enemiAttackPlayer)
        {
            players[0]->health -= 10;
        }
        std::cout << "player health : " << players[0]->health << std::endl;

        std::cout << "-----------------------------------------" << std::endl;
        window.clear();

        grid.draw(window);
        window.draw(players[0]->shape);
        for (const auto& enemy : enemies) {
            if (enemy->isAlive()) {
                window.draw(enemy->shape);
                window.draw(enemy->circle);
            }
        }
        window.display();
    }
    
    return 0;
}