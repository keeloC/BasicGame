#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <random>
#include <string>
#include <utility>
#include <vector>

int main() {
  sf::RenderWindow window(sf::VideoMode({800, 600}), "Esotilin Juego");
  window.setFramerateLimit(60);

  sf::RectangleShape player(sf::Vector2f(30.f, 30.f));
  player.setFillColor(sf::Color::Green);
  player.setPosition({
      window.getSize().x / 2.f - player.getSize().x / 2.f,
      window.getSize().y / 2.f - player.getSize().y / 2.f,
  });
  // speed

  float speed = 15.f;
  float Espeed = 2.f;

  // numero de enemigos

  int Numenemigos = 15;
  float enemieSize = 5.f;

  // reloj

  sf::Clock Clock;
  float record = 0.f;

  // random

  std::mt19937 rng(std::random_device{}());
  std::uniform_real_distribution<float> distX(0.f, 770.f);
  std::uniform_real_distribution<float> distY(0.f, 570.f);

  std::vector<sf::RectangleShape> enemies(Numenemigos);
  std::vector<sf::Vector2f> directions(Numenemigos);

  std::uniform_real_distribution<float> distDir(-1.f, 1.f);

  for (int i = 0; i < Numenemigos; i++) {
    enemies[i].setSize({enemieSize, enemieSize});
    enemies[i].setFillColor(sf::Color::Red);
    enemies[i].setPosition({distX(rng), distY(rng)});
    directions[i] = {distDir(rng), distDir(rng)};
  }
  // contador

  sf::Font font;
  font.openFromFile("/usr/share/fonts/TTF/DejaVuSansCondensed.ttf");

  sf::Text timetext(font);
  timetext.setCharacterSize(24);
  timetext.setFillColor(sf::Color::White);
  timetext.setPosition({10.f, 10.f});

  sf::Text gameOverText(font);
  gameOverText.setCharacterSize(48);
  gameOverText.setFillColor(sf::Color::Red);
  gameOverText.setString("fallasteputa\nRecord: ");
  gameOverText.setPosition(
      {window.getSize().x / 2.f - gameOverText.getLocalBounds().size.x / 2.f,
       window.getSize().y / 2.f - gameOverText.getLocalBounds().size.y / 2.f});

  // bool

  bool Gameover = false;

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    if (Gameover) {
      gameOverText.setString(
          "fallasteputa\nRecord: " + std::to_string((int)record) + "s");
      window.clear();
      window.draw(gameOverText);
      window.display();
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
        Gameover = false;
        Clock.restart();
        player.setPosition(
            {window.getSize().x / 2.f - player.getSize().x / 2.f,
             window.getSize().y / 2.f - player.getSize().y / 2.f});
        for (int j = 0; j < Numenemigos; j++) {
          enemies[j].setPosition({distX(rng), distY(rng)});
          directions[j] = {distDir(rng), distDir(rng)};
        }
      }
    } else {

      // movimiento con el teclado
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
        player.move({-speed, 0.f});
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
        player.move({speed, 0.f});
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
        player.move({0.f, speed});
      }
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
        player.move({0.f, -speed});
      }
      // limite borde
      if (player.getPosition().x < 0.f) {
        player.setPosition({0.f, player.getPosition().y});
      }
      if (player.getPosition().x > 770.f) {
        player.setPosition({770.f, player.getPosition().y});
      }
      if (player.getPosition().y < 0.f) {
        player.setPosition({player.getPosition().x, 0.f});
      }
      if (player.getPosition().y > 570.f) {
        player.setPosition({player.getPosition().x, 570.f});
      }
      // enemigos que salen de la pantalla
      for (int i = 0; i < Numenemigos; i++) {
        for (int j = i + 1; j < Numenemigos; j++) {
          if (enemies[i].getGlobalBounds().findIntersection(
                  enemies[j].getGlobalBounds())) {
            std::swap(directions[i], directions[j]);
          }
        }
        enemies[i].move(directions[i] * Espeed);
        if (enemies[i].getPosition().x > 800.f) {
          enemies[i].setPosition({0.f, enemies[i].getPosition().y});
        }
        if (enemies[i].getPosition().x < 0.f) {
          enemies[i].setPosition({800.f, enemies[i].getPosition().y});
        }
        if (enemies[i].getPosition().y > 600.f) {
          enemies[i].setPosition({enemies[i].getPosition().x, 0.f});
        }
        if (enemies[i].getPosition().y < 0.f) {
          enemies[i].setPosition({enemies[i].getPosition().x, 600.f});
        }
        if (player.getGlobalBounds().findIntersection(
                enemies[i].getGlobalBounds())) {
          player.setPosition(
              {window.getSize().x / 2.f - player.getSize().x / 2.f,
               window.getSize().y / 2.f - player.getSize().y / 2.f});
          for (int j = 0; j < Numenemigos; j++) {
            enemies[j].setPosition({distX(rng), distY(rng)});
            directions[j] = {distDir(rng), distDir(rng)};
          }
          Clock.restart();
          Gameover = true;
        }
      }

      float currentTime;
      currentTime = Clock.getElapsedTime().asSeconds();
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