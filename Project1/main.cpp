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

    Player player(400, 400, 10);
    std::vector<std::shared_ptr<Entity>> enemies = { std::make_shared<Enemy>(100, 100, 10), std::make_shared<Enemy>(700, 100, 100) };
    std::vector<std::shared_ptr<Enemy>> trueEnemies = { std::dynamic_pointer_cast<Enemy>(enemies[0]), std::dynamic_pointer_cast<Enemy>(enemies[1]) };
    
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
            InheritFromEveryone::makeTree(root1, root2, sequenceEnemyOne1, sequenceEnemyOne2, sequenceEnemyOne3, sequenceEnemyOne4, bb, *trueEnemies[i], playerDetected, playerInsight, lowHP, grid);
        else
            InheritFromEveryone::makeTree(root3, root4, sequenceEnemyTwo1, sequenceEnemyTwo2, sequenceEnemyTwo3, sequenceEnemyTwo4, bb, *trueEnemies[i], playerDetected, playerInsight, lowHP, grid);
    }

    GOAPAgent agent;
    std::vector<Goal> goals;
    goals = { Goal::Patrouiller, Goal::Chasser, Goal::Chercher, Goal::Fuir };

    std::cout << "Etat initial de l'agent:\n";
    agent.PrintState();

    std::cout << "\nL'agent commence ses actions...\n";
    agent.PerformActions(goals);  // L'agent va chercher de la nourriture, puis manger

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
            if (enemy->isAlive()) {
                // Mise à jour du behavior tree et exécution de ce dernier
                InheritFromEveryone::executeTree(root1, bb, playerDetected, playerInsight, lowHP);
                // Mise à jour de la position du joueur pour les ennemis
                enemy->update(deltaTime, grid, enemies);
            }
        }

        for (auto& enemy : trueEnemies) {
            if (enemy->isAlive()) {
                enemy->playerPos = player.shape.getPosition();
                // Mise à jour des valeurs booléennes des ennemis
                if (enemy->playerDetected) playerDetected = true;
                else                       playerDetected = false;
                if (enemy->playerInsight)  playerInsight = true;
                else                       playerInsight = false;
                if (enemy->lowHP)          lowHP = true;
                else                       lowHP = false;
            }
        }

        player.update(deltaTime, grid, enemies);
    
        window.clear();

        grid.draw(window);
        window.draw(player.shape);
        for (const auto& enemy : enemies) {
            if (enemy->isAlive()) {
                window.draw(enemy->shape);
            }
        }
        window.display();
    }
    
    return 0;
}