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

   Player player(200, 400);
   std::vector<Enemy> enemies = { Enemy(100, 100), Enemy(700, 100) };
   int playerDetected = 0;
   int playerInsight = 0;
   int lowHP = 0;
   Grid grid;
   grid.loadFromFile("map.txt");
   //BTNode->this(grid); ----------------------------------------------------------------------------------

   sf::Clock clock;

   while (window.isOpen()) {
       sf::Time dt = clock.restart();
       float deltaTime = dt.asSeconds();

       sf::Event event;
       while (window.pollEvent(event)) {
           if (event.type == sf::Event::Closed)
               window.close();
       }

       player.update(deltaTime, grid);

       for (auto enemy : enemies) {
           if (enemy.playerDetected) { playerDetected = 1; }
           else { playerDetected = 0; }
           if (enemy.playerInsight) { playerInsight = 1; }
           else { playerInsight = 0; }
           if (enemy.lowHP) { lowHP = 1; }
           else { lowHP = 0; }
           BTNode::makeTree(enemy, playerDetected, playerInsight, lowHP);
           //-----réinitialise les valeurs--------(pas utile)
           playerDetected = 0;
           playerInsight = 0;
           lowHP = 0;
       }

       window.clear();
       grid.draw(window);
       window.draw(player.shape);
       for (const auto& enemy : enemies)
           window.draw(enemy.shape);
       window.display();
   }

   return 0;
}