// Copyright 2021 Aily Walker aily@bu.edu
// Copyright 2021 Sophia Brocoum sbrocoum@bu.edu
// Copyright 2021 Molly McGowan mamcg@bu.edu
// Copyright 2021 Moriah White whitemo@bu.edu

#include <math.h>
#include "Textbox.h"
#include <unistd.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "func_backend.cpp"
using std::cin;
using std::cout;
using std::vector;


class TileMap : public sf::Drawable, public sf::Transformable {
 public:
  // class from SFML example
  bool load(const std::string& spacetile, sf::Vector2u tileSize,
            const int* tiles, unsigned int w, unsigned int h) {
      // load the tileset texture
      if (!tile_texture.loadFromFile(spacetile))
          return false;

      // resize the vertex array to fit the level size
      m_vertices.setPrimitiveType(sf::Quads);
      m_vertices.resize(w * h * 4);

      // populate the vertex array, with one quad per tile
      for (unsigned int i = 0; i < w; ++i)
          for (unsigned int j = 0; j < h; ++j) {
              // get the current tile number
              int tileNumber = tiles[i + j * w];

              // find its position in the tileset texture
              int tu = tileNumber % (tile_texture.getSize().x / tileSize.x);
              int tv = tileNumber / (tile_texture.getSize().x / tileSize.x);

              // get a pointer to the current tile's quad
              sf::Vertex* quad = &m_vertices[(i + j * w) * 4];

              // define its 4 corners
              quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
              quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
              quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
              quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

              // define its 4 texture coordinates
              quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
              quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
              quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
              quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
          }

      return true;
  }

 private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // transform, apply texture & draw
        states.transform *= getTransform();
        states.texture = &tile_texture;
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture tile_texture;
};

bool isNumber(string str) {
    for (auto c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}


int main() {
  TileMap tile_map;

  // Load music
  sf::Music music;
  if (!music.openFromFile("data/music/soundtrack.wav"))
    return -1;
  music.play();

  // define the tile_vec with an array of tile indices
  int tile_arr[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 2, 0, 11, 11, 11, 11, 11, 11, 11, 0, 7, 0,
    0, 0, 0, 11, 5, 11, 1, 11, 2, 11, 0, 0, 0,
    0, 1, 0, 11, 11, 11, 11, 11, 11, 11, 0, 8, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  };

  // create the tile_map from the level definition
  if (!tile_map.load("data/images/spacetile3.png",
    sf::Vector2u(50, 50), tile_arr, 13, 11))
      return -1;

  // Player starts off with 100 coins
  signed int coins = 100, bank = 100;
  int bet; string bet_str;
  unsigned int microsec = 1000000;
  int matches = 0;

  // Displaying the title & instructions
  sf::Font font;
  font.loadFromFile("OpenSans-BoldItalic.ttf");
  sf::Text text;
  text.setFont(font);
  text.setString("Click anywhere to begin!");
  text.setCharacterSize(24);
  text.setFillColor(sf::Color::Red);
  text.setStyle(sf::Text::Bold);
  text.setOutlineColor(sf::Color::White);
  text.setOutlineThickness(5);
  text.setPosition(175, 50);

  // displaying coin bank
  sf::Text ctext;
  ctext.setFont(font);
  ctext.setString("Coins: " + std::to_string(coins));
  ctext.setCharacterSize(30);
  ctext.setFillColor(sf::Color::Red);
  ctext.setOutlineColor(sf::Color::White);
  ctext.setOutlineThickness(5);
  ctext.setStyle(sf::Text::Bold);
  ctext.setPosition(250, 450);
  
  // displaying prize message
  sf::Text prize;
  prize.setFont(font);
  prize.setCharacterSize(40);
  prize.setFillColor(sf::Color::Green);
  prize.setOutlineColor(sf::Color::White);
  prize.setOutlineThickness(5);
  prize.setStyle(sf::Text::Bold);
  prize.setPosition(230, 350);

  // Initialize textbox
  Textbox textbox(24, sf::Color::Black, true);
  textbox.setPosition({265, 100});
  textbox.setLimit(true, 7);
  textbox.setFont(font);

    // Bet input textbox
  sf::RectangleShape numbet(sf::Vector2f(120, 30));
  numbet.setOutlineThickness(5);
  numbet.setOutlineColor(sf::Color::White);
  numbet.setPosition(265, 100);

  bool clicked = false;  // set mouseclick to false
  bool isbreak = false;

  // create the window
  sf::RenderWindow window(sf::VideoMode(650, 550), "Slot Machine");
  window.clear();
  window.draw(tile_map);
  window.draw(text);
  window.draw(ctext);
  window.display();

  auto position = sf::Mouse::getPosition(window);

  // run the main loop
  while (window.isOpen()) {

    // handle events
    sf::Event event;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
      textbox.setSelected(true);
    }

    while (window.pollEvent(event)) {
      switch (event.type) {

        case sf::Event::Closed:
          window.close();

        case sf::Event::TextEntered:
          if (clicked)
            textbox.typedOn(event);

        case sf::Event::MouseButtonPressed:
          position = sf::Mouse::getPosition(window);
          if ((position.x < 650 && position.y < 550)) {clicked = true;}

          if (clicked) {
            window.clear();
            
            text.setString("Place a bet of 10 coins or more.");
            text.setPosition(140, 50);

            window.draw(tile_map);
            window.draw(text);
            window.draw(ctext);
            window.draw(numbet);
            textbox.drawTo(window);
            window.display();
          }

        case sf::Event::KeyReleased:
          // if enter key is released
          if (event.key.code == sf::Keyboard::Return) {
            if (clicked) {
              bet_str = textbox.getText();

              window.clear();
              // checks if bet is valid
              if (isNumber(bet_str)) {
                bet = std::stoi(bet_str);

                // Checks if bet is valid
                if ((bet < 10) or (bet > coins)) {
                  isbreak = true;
                  window.clear();
                  window.draw(tile_map);
                  window.draw(ctext);
                     
                  sf::Text vtext;
                  vtext.setFont(font);
                  vtext.setString("Invalid. Click to enter new bet:");
                  vtext.setCharacterSize(24);
                  vtext.setFillColor(sf::Color::Red);
                  vtext.setStyle(sf::Text::Bold);
                  vtext.setOutlineColor(sf::Color::White);
                  vtext.setOutlineThickness(5);
                  vtext.setPosition(145, 50);
                  window.draw(vtext);

                  window.display();
                  usleep(1 * microsec);  // sleeps
                }

                if (isbreak) {
                  isbreak = false;
                  break;
                }

                // isvalid(bet, coins);
                // subtract # of coins in GUI
                coins -= bet;
              } else {
                break;
              }

              // Change slots to "?"
              tile_arr[56] = 12;
              tile_arr[58] = 12;
              tile_arr[60] = 12;
              if (!tile_map.load("data/images/spacetile3.png",
                sf::Vector2u(50, 50), tile_arr, 13, 11))
                return -1;

              // Updating the GUI
              window.clear();
              window.draw(tile_map);
              ctext.setString("Coins: " + std::to_string(coins));
              window.draw(ctext);
              window.display();
              usleep(1 * microsec);  // sleeps

              // pass bet to spin function
              vector<int> output = spin(coins, bet);
              coins = output.at(0);
              matches = output.at(4);

              // assign slot machine outcome to slot images
              for (int i = 0; i <= 4; i+=2) {
                int offset = 56;

                tile_arr[offset + i] = output.at(i/2 + 1);
                if (!tile_map.load("data/images/spacetile3.png",
                  sf::Vector2u(50, 50), tile_arr, 13, 11))
                  return -1;
                window.clear();
                window.draw(ctext);
                window.draw(tile_map);
                window.display();
                usleep(1.5 * microsec);  // sleeps
              }

              // Updating the GUI
              window.clear();
              window.draw(tile_map);
              ctext.setString("Coins: " + std::to_string(coins));
              text.setString("Click again to place another bet.");
              text.setPosition(135, 50);

              if (matches == 1) {
                prize.setString("YOU WON!");
                window.draw(prize);
              } else if (matches == 3) {
                prize.setString("JACKPOT");
                window.draw(prize);
              }
              matches = 0;

              window.draw(text);
              window.draw(ctext);
              window.display();

              if (coins < 10) {
                // Game over screen
                // int tile_arr[] = {0};
                // if (!tile_map.load("data/images/spacetile3.png",
                //   sf::Vector2u(50, 50), tile_arr, 13, 11))
                // return -1;

                text.setString("GAME OVER");
                text.setCharacterSize(40);
                text.setPosition(210, 50);
                window.clear();
                window.draw(ctext);
                window.draw(tile_map);
                window.draw(text);
                window.display();

                usleep(2 * microsec);  // Pause for 2 sec
                window.close();
                break;
              }

            }
          }
      }
    }
  }

  return 0;
}