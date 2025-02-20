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

    std::vector<sf::Vector2f> patrolEnemyOne = {
        sf::Vector2f(80, 80),
        sf::Vector2f(80, 200),
        sf::Vector2f(320, 200),
        sf::Vector2f(320, 80)
    };

    std::vector<sf::Vector2f> patrolEnemyTwo = {
        sf::Vector2f(680, 80),
        sf::Vector2f(680, 200),
        sf::Vector2f(440, 200),
        sf::Vector2f(440, 80)
    };

    std::shared_ptr<Entity> player = std::make_shared<Player>(400.f, 400.f, 50);
    std::vector<std::shared_ptr<Entity>> players = { player };
    std::vector<std::shared_ptr<Enemy>> enemiesDerived = {std::make_shared<Enemy>(100.f, 100.f, 1, patrolEnemyOne), std::make_shared<Enemy>(700.f, 100.f, 100, patrolEnemyTwo)};
    std::vector<std::shared_ptr<Entity>> enemiesBase = { enemiesDerived[0], enemiesDerived[1] };
    
    auto root = std::make_unique<SelectorNode>();
    auto root2 = std::make_unique<SelectorNode>();

    Grid grid;
    grid.loadFromFile("map.txt");

    Blackboard bb;
    InheritFromEveryone method;
    InheritFromEveryone method2;

    for (int i = 0; i < 2; ++i) {
        if (i == 0)
            method.makeTree(root, bb, enemiesDerived[i], enemiesDerived[i]->playerDetected, enemiesDerived[i]->playerInsight, enemiesDerived[i]->lowHP, grid);
        else
            method2.makeTree(root2, bb, enemiesDerived[i], enemiesDerived[i]->playerDetected, enemiesDerived[i]->playerInsight, enemiesDerived[i]->lowHP, grid);
    }

    GOAPAgent agent;
    std::vector<Goal> goals;
    goals = { Goal::Patrouiller, Goal::Chasser, Goal::Chercher, Goal::Fuir };

    std::cout << "Etat initial de l'agent:\n";
    std::cout << "-----------------------------------------" << std::endl;
    agent.PrintState();

    std::cout << "\nL'agent commence ses actions...\n";
    try { agent.PerformActions(goals); }
    catch (const std::exception& e) { std::cerr << "Exception attrapée : " << e.what() << std::endl; }

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

        player->update(deltaTime, grid, enemiesBase);

        int counter = 1;
        for (auto& enemy : enemiesDerived)
        {
            if (enemy->isAlive())
            {
                enemy->update(deltaTime, grid, players);
                if(counter == 1)
                    method.executeTree(root, bb, enemy->playerDetected, enemy->playerInsight, enemy->lowHP, deltaTime, *enemy);
                else
                    method2.executeTree(root2, bb, enemy->playerDetected, enemy->playerInsight, enemy->lowHP, deltaTime, *enemy);
                std::cout << std::endl;
                if (enemy->getStatutAtk() && (enemyAtkCD == 0) && player->isAlive())
                {
                    player->health -= 10;
                    if (player->health <= 0) 
                    {
                        player->health = 0;
                    }
                }
                counter++;
            }
        }

        enemyAtkCD += deltaTime;
        if (enemyAtkCD >= 1) {
            enemyAtkCD = 0;
        }

        window.clear();
        grid.draw(window);
        window.draw(player->shape);
        for (const auto& enemy : enemiesDerived) {
            if (enemy->isAlive()) {
                window.draw(enemy->shape);
                window.draw(enemy->circle);
            }
        }
        window.display();

    }
    return 0;
}