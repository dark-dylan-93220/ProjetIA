#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Enemy.hpp"
#include "GOAP.hpp"
#include "Grid.hpp"
#include "BT.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jeu SFML - IA Ennemis");
    window.setFramerateLimit(60);

    Clock clock;
    Event event;

    float enemyAtkCD = 0;
    float deltaTime = 0;

    // Note : Les 3 ennemis utilisent l'aglorithme A* pour la recherche de chemin

    // Ennemi FSM
    vector<Vector2f> patrolEnemyOne = {
        Vector2f(80, 80),
        Vector2f(80, 200),
        Vector2f(320, 200),
        Vector2f(320, 80)
    };

    // Ennemi behaviour tree
    vector<Vector2f> patrolEnemyTwo = {
        Vector2f(680, 80),
        Vector2f(680, 200),
        Vector2f(440, 200),
        Vector2f(440, 80)
    };

    // Ennemi GOAP
    vector<Vector2f> patrolEnemyThree = {
        Vector2f(680, 480),
        Vector2f(680, 360),
        Vector2f(440, 360),
        Vector2f(440, 480)
    };

    // Déclarations du joueur et des ennemis
    shared_ptr<Entity> player = make_shared<Player>(400.f, 400.f, 50);
    vector<shared_ptr<Entity>> players = { 
        player
    };
    vector<shared_ptr<Enemy>> enemiesDerived = {
        make_shared<Enemy>(80.f, 80.f, 1, patrolEnemyOne, "FSM"), 
        make_shared<Enemy>(680.f, 80.f, 100, patrolEnemyTwo, "BehaviourTree"), 
        make_shared<Enemy>(680.f, 520.f, 100, patrolEnemyThree, "GOAP")
    };
    vector<shared_ptr<Entity>> enemiesBase = { 
        enemiesDerived[0], 
        enemiesDerived[1], 
        enemiesDerived[2] 
    };
    
    // Grille du fond du jeu
    Grid grid;
    grid.loadFromFile("map.txt");

    // Déclaration du behaviour tree (Ennemi n°2)
    auto root = make_unique<SelectorNode>();
    Blackboard bb;
    InheritFromEveryone BehaviourTree;
    BehaviourTree.makeTree(root, bb, enemiesDerived[1], enemiesDerived[1]->playerDetected, enemiesDerived[1]->playerInsight, enemiesDerived[1]->lowHP, grid);

    // Déclaration de la méthode GOAP (Ennemi n°3)
    GOAPAgent agent;
        // SpecificAction(cout, conditions, effets, nom);
    PatrollingAction patrol(5,  {"Tout"},        {"Patrolling", "Tout"}, "Patrolling");
    ChaseAction      chase (15, {"Patrolling"},  {"Chase", "Tout"},      "Chase");
    FleeAction       flee  (0,  {"Tout"},        {"Flee", "Tout"},       "Flee");
    AttackAction     attack(5,  {"Chase"},       {"Attack", "Tout"},     "Attack");
        // Vecteurs des actions possibles
    vector<SpecificAction> actions = { patrol, chase, flee, attack };
    vector<string> effets = { "Patrolling", "Tout" };
    State goalState(effets); // But par défaut

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();
        cout << deltaTime << endl;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        player->update(deltaTime, grid, enemiesBase);

        int enemyIndex = 0;
        for (auto& enemy : enemiesDerived) 
        {
            if (enemy->isAlive())
            {
                enemy->update(deltaTime, grid, players);

                if (enemy->identifiant == "FSM") {
                    enemy->FSMAndGOAPUpdate(deltaTime, enemy->playerDetected, enemy->playerInsight, enemy->lowHP, players[0]->shape.getPosition(), goalState, agent.state);
                }
                else if (enemy->identifiant == "BehaviourTree") {
                    BehaviourTree.executeTree(root, bb, enemy->playerDetected, enemy->playerInsight, enemy->lowHP, deltaTime);
                }
                else if (enemy->identifiant == "GOAP") {
                    enemy->FSMAndGOAPUpdate(deltaTime, enemy->playerDetected, enemy->playerInsight, enemy->lowHP, players[0]->shape.getPosition(), goalState, agent.state);
                    // Méthode try-catch pour la gestion d'erreurs
                    try {
                        agent.PerformActions(goalState, actions, *enemy, deltaTime);
                    }
                    catch (const exception& e) {
                        cerr << "Exception attrapee : " << e.what() << endl;
                    }
                }

                if (enemy->getStatutAtk() && (enemyAtkCD == 0) && player->isAlive()) 
                {
                    player->health -= 10;
                    if (player->health <= 0) 
                    {
                        player->health = 0;
                    }
                }
                enemyIndex++;
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
                window.draw(enemy->circle);
                window.draw(enemy->shape);
            }
        }
        window.display();

    }

    return 0;
}