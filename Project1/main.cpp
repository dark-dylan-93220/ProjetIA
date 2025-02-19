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

    std::shared_ptr<Entity> player = std::make_shared<Player>(400.f, 400.f, 50);
    std::vector<std::shared_ptr<Entity>> players = { player };
    std::vector<std::shared_ptr<Entity>> enemies = { std::make_shared<Enemy>(100.f, 100.f, 1), std::make_shared<Enemy>(700.f, 100.f, 100) };
    
    auto root = std::make_unique<SelectorNode>();

    Grid grid;
    grid.loadFromFile("map.txt");

    Blackboard bb;

    for (int i = 0; i < 2; ++i) {
        if (i == 0)
            InheritFromEveryone::makeTree(root, bb, enemies[i], std::dynamic_pointer_cast<Enemy>(enemies[i])->playerDetected, std::dynamic_pointer_cast<Enemy>(enemies[i])->playerInsight, std::dynamic_pointer_cast<Enemy>(enemies[i])->lowHP, grid);
        else
            InheritFromEveryone::makeTree(root, bb, enemies[i], std::dynamic_pointer_cast<Enemy>(enemies[i])->playerDetected, std::dynamic_pointer_cast<Enemy>(enemies[i])->playerInsight, std::dynamic_pointer_cast<Enemy>(enemies[i])->lowHP, grid);
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

        player->update(deltaTime, grid, enemies);

        for (auto& enemy : enemies)
        {
            if (std::dynamic_pointer_cast<Enemy>(enemy) == enemy) 
            {
                if (enemy->isAlive())
                {
                    goals = { Goal::Patrouiller, Goal::Chasser, Goal::Chercher, Goal::Fuir };
                    agent.PerformActions(goals);
                    std::cout << "Etat ennemi a l'adresse " << enemy.get() << " : ";
                    InheritFromEveryone::executeTree(root, bb, std::dynamic_pointer_cast<Enemy>(enemy)->playerDetected, std::dynamic_pointer_cast<Enemy>(enemy)->playerInsight, std::dynamic_pointer_cast<Enemy>(enemy)->lowHP);
                    std::cout << std::endl;
                    enemy->update(deltaTime, grid, players);
                    if (enemy->getStatutAtk() && (enemyAtkCD == 0) && player->isAlive())
                    {
                        player->health -= 10;
                        if (player->health <= 0) 
                        {
                            player->health = 0;
                        }
                    }
                }
            }
        }
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