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

    float enemyAtkCD = 0;
    float deltaTime = 0;

    std::vector<std::shared_ptr<Entity>> players = { std::make_shared<Player>(400, 400, 50) };
    std::vector<std::shared_ptr<Entity>> enemies = { std::make_shared<Enemy>(100, 100, 1), std::make_shared<Enemy>(700, 100, 100) };

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
    std::vector<Goal> goals;
    goals = { Goal::Patrouiller, Goal::Chasser, Goal::Chercher, Goal::Fuir };

    std::cout << "Etat initial de l'agent:\n";
    std::cout << "-----------------------------------------" << std::endl;
    agent.PrintState();

    std::cout << "\nL'agent commence ses actions...\n";
    agent.PerformActions(goals);

    std::cout << "\nEtat de l'agent apres avoir effectue les actions:\n";
    std::cout << "-----------------------------------------" << std::endl;
    agent.PrintState();

    while (window.isOpen()) 
    {
        deltaTime = clock.restart().asSeconds();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        players[0]->update(deltaTime, grid, enemies);

        for (auto& enemy : enemies)
        {
            if (std::dynamic_pointer_cast<Enemy>(enemy) == enemy) 
            {
                std::cout << "Etat ennemi a l'adresse " << enemy.get() << " : ";
                if (enemy->isAlive())
                {
                    InheritFromEveryone::executeTree(root1, bb, std::dynamic_pointer_cast<Enemy>(enemy)->playerDetected, std::dynamic_pointer_cast<Enemy>(enemy)->playerInsight, std::dynamic_pointer_cast<Enemy>(enemy)->lowHP);
                    enemy->update(deltaTime, grid, players);
                    if (enemy->getStatutAtk() && (enemyAtkCD == 0)) 
                    {
                        players[0]->health -= 10;
                        if (players[0]->health <= 0) 
                        {
                            players[0]->health = 0;
                        }
                    }
                }
            }
        }

        std::cout << "Player's health : " << players[0]->health << std::endl;
        std::cout << "-----------------------------------------" << std::endl;

        enemyAtkCD += deltaTime;
        if (enemyAtkCD >= 1) {
            enemyAtkCD = 0;
        }

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