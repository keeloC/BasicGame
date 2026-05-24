#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <random>
#include <string>
#include <utility>
#include <vector>
int main() {
  sf::RenderWindow window(sf::VideoMode(800, 600), "Esotilin Juego");
  window.setFramerateLimit(60);

  float SizePlayer = 5.f;

  sf::RectangleShape player(sf::Vector2f(SizePlayer, SizePlayer));
  player.setFillColor(sf::Color::Green);

  player.setPosition(window.getSize().x / 2.f - player.getSize().x / 2.f,
                     window.getSize().y / 2.f - player.getSize().y / 2.f);

  float speed = 13.f;
  float Espeed = 2.f;

  int Numenemigos = 15;
  float enemieSize = 5.f;

  sf::Clock Clock;
  float record = 0.f;

  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<float> distX(0.f, 770.f);
  std::uniform_real_distribution<float> distY(0.f, 570.f);
  std::uniform_real_distribution<float> distDir(-1.f, 1.f);

  std::vector<sf::RectangleShape> enemies(Numenemigos);
  std::vector<sf::Vector2f> directions(Numenemigos);

  for (int i = 0; i < Numenemigos; i++) {
    enemies[i].setSize(sf::Vector2f(enemieSize, enemieSize));
    enemies[i].setFillColor(sf::Color::Red);

    enemies[i].setPosition(distX(rng), distY(rng));

    directions[i] = sf::Vector2f(distDir(rng), distDir(rng));
  }

  sf::Font font;

  if (!font.loadFromFile("/usr/share/fonts/TTF/DejaVuSansCondensed.ttf")) {
    return -1;
  }

  sf::Text timetext;
  timetext.setFont(font);
  timetext.setCharacterSize(24);
  timetext.setFillColor(sf::Color::White);
  timetext.setPosition(10.f, 10.f);

  sf::Text gameOverText;
  gameOverText.setFont(font);
  gameOverText.setCharacterSize(48);
  gameOverText.setFillColor(sf::Color::Red);
  gameOverText.setString("fallasteputa\nRecord: ");

  gameOverText.setPosition(
      window.getSize().x / 2.f - gameOverText.getLocalBounds().width / 2.f,
      window.getSize().y / 2.f - gameOverText.getLocalBounds().height / 2.f);

  bool Gameover = false;

  while (window.isOpen()) {

    sf::Event event;

    while (window.pollEvent(event)) {

      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    if (Gameover) {

      gameOverText.setString(
          "fallasteputa\nRecord: " + std::to_string((int)record) + "s");

      window.clear();
      window.draw(gameOverText);
      window.display();

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {

        Gameover = false;

        Clock.restart();

        player.setPosition(window.getSize().x / 2.f - player.getSize().x / 2.f,
                           window.getSize().y / 2.f - player.getSize().y / 2.f);

        for (int j = 0; j < Numenemigos; j++) {

          enemies[j].setPosition(distX(rng), distY(rng));

          directions[j] = sf::Vector2f(distDir(rng), distDir(rng));
        }
      }

    } else {

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        player.move(-speed, 0.f);
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        player.move(speed, 0.f);
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        player.move(0.f, speed);
      }

      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        player.move(0.f, -speed);
      }

      if (player.getPosition().x < 0.f) {
        player.setPosition(0.f, player.getPosition().y);
      }

      if (player.getPosition().x > 770.f) {
        player.setPosition(770.f, player.getPosition().y);
      }

      if (player.getPosition().y < 0.f) {
        player.setPosition(player.getPosition().x, 0.f);
      }

      if (player.getPosition().y > 570.f) {
        player.setPosition(player.getPosition().x, 570.f);
      }

      for (int i = 0; i < Numenemigos; i++) {

        for (int j = i + 1; j < Numenemigos; j++) {

          if (enemies[i].getGlobalBounds().intersects(
                  enemies[j].getGlobalBounds())) {

            std::swap(directions[i], directions[j]);
          }
        }

        enemies[i].move(directions[i].x * Espeed, directions[i].y * Espeed);

        if (enemies[i].getPosition().x > 800.f) {
          enemies[i].setPosition(0.f, enemies[i].getPosition().y);
        }

        if (enemies[i].getPosition().x < 0.f) {
          enemies[i].setPosition(800.f, enemies[i].getPosition().y);
        }

        if (enemies[i].getPosition().y > 600.f) {
          enemies[i].setPosition(enemies[i].getPosition().x, 0.f);
        }

        if (enemies[i].getPosition().y < 0.f) {
          enemies[i].setPosition(enemies[i].getPosition().x, 600.f);
        }

        if (player.getGlobalBounds().intersects(enemies[i].getGlobalBounds())) {

          player.setPosition(
              window.getSize().x / 2.f - player.getSize().x / 2.f,
              window.getSize().y / 2.f - player.getSize().y / 2.f);

          for (int j = 0; j < Numenemigos; j++) {

            enemies[j].setPosition(distX(rng), distY(rng));

            directions[j] = sf::Vector2f(distDir(rng), distDir(rng));
          }

          Clock.restart();
          Gameover = true;
        }
      }

      float currentTime = Clock.getElapsedTime().asSeconds();

      if (currentTime > record) {
        record = currentTime;
      }

      timetext.setString("Tiempo: " + std::to_string((int)currentTime) + "s" +
                         "\nRecord: " + std::to_string((int)record) + "s");

      Espeed = 5.f + currentTime * 0.5f;

      window.clear();

      for (int i = 0; i < Numenemigos; i++) {
        window.draw(enemies[i]);
      }

      window.draw(player);
      window.draw(timetext);

      window.display();
    }
  }

  return 0;
}
