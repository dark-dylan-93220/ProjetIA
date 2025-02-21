#include <SFML/Graphics.hpp>

#include "Player.hpp"
#include "Enemy.hpp"
#include "GOAP.hpp"
#include "Grid.hpp"
#include "BT.hpp"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Jeu SFML - IA Ennemis", Style::Titlebar | Style::Close);
    window.setFramerateLimit(60);

    Clock clock;
    Event event;

    float enemyAtkCD = 0;
    float deltaTime = 0;

    bool isGameOver = false; 
    int colorGameOver = 0;
    Texture gameOverTexture;
    Sprite gameOverSprite;
    gameOverTexture.loadFromFile("assets/gameOver.jpg");
    gameOverSprite.setTexture(gameOverTexture);
    gameOverSprite.setPosition(WINDOW_WIDTH / 2 - gameOverTexture.getSize().x / 2, WINDOW_HEIGHT / 2 - gameOverTexture.getSize().y / 2);

    RectangleShape gameOverRect;
    gameOverRect.setFillColor(Color(0, 0, 0, colorGameOver));
    gameOverRect.setPosition(0, 0);
    gameOverRect.setSize(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));

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
    shared_ptr<Player> player = make_shared<Player>(400.f, 400.f, 80);
    vector<shared_ptr<Entity>> players = { 
        player
    };
    vector<shared_ptr<Enemy>> enemiesDerived = {
        make_shared<Enemy>(80.f, 80.f, 85, patrolEnemyOne, "FSM"), 
        make_shared<Enemy>(680.f, 80.f, 85, patrolEnemyTwo, "BehaviourTree"), 
        make_shared<Enemy>(680.f, 520.f, 85, patrolEnemyThree, "GOAP")
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
    PatrollingAction patrol(5,  {"Tout"}, {"Patrolling", "Tout"}, "Patrolling");
    ChaseAction      chase (15, {"Tout"}, {"Chase", "Tout"},      "Chase");
    FleeAction       flee  (0,  {"Tout"}, {"Flee", "Tout"},       "Flee");
    AttackAction     attack(5,  {"Tout"}, {"Attack", "Tout"},     "Attack");
        // Vecteurs des actions possibles
    vector<SpecificAction> actions = { patrol, chase, attack, flee };
    vector<string> effets = { "Patrolling", "Tout" };
    State goalState(effets); // But par défaut

    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        if (!isGameOver) {
            cout << "HP joueur : " << player->health << endl;
            player->update(deltaTime, grid, enemiesBase);
            int enemyIndex = 0;
            for (auto& enemy : enemiesDerived)
            {
                if (enemy->isAlive())
                {
                    enemy->update(deltaTime, grid, players);

                    if (enemy->identifiant == "FSM") {
                        cout << "-----------------------------" << endl;
                        cout << "HP FSM : " << enemy->health << endl;
                        enemy->FSMAndGOAPUpdate(deltaTime, enemy->playerDetected, enemy->playerInsight, enemy->lowHP, players[0]->shape.getPosition(), goalState, agent.state);
                    }
                    else if (enemy->identifiant == "BehaviourTree") {
                        cout << "-----------------------------" << endl;
                        cout << "HP Behavior Tree : " << enemy->health << endl;
                        BehaviourTree.executeTree(root, bb, enemy->playerDetected, enemy->playerInsight, enemy->lowHP, deltaTime);
                    }
                    else if (enemy->identifiant == "GOAP") {
                        cout << "-----------------------------" << endl;
                        cout << "HP GOAP : " << enemy->health << endl;
                        enemy->FSMAndGOAPUpdate(deltaTime, enemy->playerDetected, enemy->playerInsight, enemy->lowHP, players[0]->shape.getPosition(), goalState, agent.state);
                        // Méthode try-catch pour la gestion d'erreurs
                        try {
                            agent.PerformActions(goalState, actions, *enemy, deltaTime);
                            agent.PrintState(agent.state);
                        }
                        catch (const exception& e) {
                            cerr << "Exception attrapee : " << e.what() << endl;
                        }
                    }

                    if (enemy->getStatutAtk() && player->isAlive() && !player->shieldActive && enemy->attackCooldown >= 1.f) {
                        enemy->attack();
                        player->takeDamage(10);
                    }
                    enemyIndex++;
                }
            }
            cout << "---------- FIN DE BOUCLE ----------" << endl;
        }
        
        if (!player->isAlive()) {
            isGameOver = true;
            colorGameOver += 2;
            gameOverRect.setFillColor(Color(0,0,0,colorGameOver));
            if (colorGameOver >= 255) {
                gameOverRect.setFillColor(Color(0, 0, 0, 255));
            }
        }

        window.clear();
        grid.draw(window);
        window.draw(player->shape);
        for (const auto& enemy : enemiesDerived) {
            if (enemy->isAlive()) {
                window.draw(enemy->circle);
                window.draw(enemy->shape);
                window.draw(enemy->name);
            }
        }
        if (isGameOver) {
            window.draw(gameOverRect);
            if (colorGameOver >= 255) {
                window.draw(gameOverSprite);
            }
        }
        window.display();
    }

    return 0;
}