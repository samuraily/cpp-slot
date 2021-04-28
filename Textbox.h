#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include<iostream>
#include <string>
#include<sstream>

using std::string;
using std::to_string;
using std::cout;

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

// From youtube tutorial
class Textbox {
 public:
  Textbox(int size, sf::Color color, bool sel) {
    textbox.setCharacterSize(size);
    textbox.setFillColor(color);
    isSelected = sel;

    if (sel) {
      textbox.setString("_");
    } else {
      textbox.setString("");
    }
  }

  void setFont(sf::Font &font) {
    textbox.setFont(font);
  }

  void setPosition(sf::Vector2f pos) {
    textbox.setPosition(pos);
  }

  void setLimit(bool ToF) {
    hasLimit = ToF;
  }

  void setLimit(bool ToF, int lim) {
    hasLimit = ToF;
    limit = lim - 1;
  }

  void setSelected(bool sel) {
    isSelected = sel;
    if (!sel) {
      std::string t = text.str();
      std::string newT = "";
      for (int i = 0; i < t.length(); i++) {
        newT += t.at(i);
      }
      textbox.setString(newT);
    }
  }

  std::string getText() {
    return text.str();
  }

  void drawTo(sf::RenderWindow &window) {
    window.draw(textbox);
  }

  void typedOn(sf::Event input) {
    if (isSelected) {
      int charTyped = input.text.unicode;
      if (charTyped < 128) {
        if (hasLimit) {
          if (text.str().length() <= limit) {
            inputLogic(charTyped);
          } else if (text.str().length() > limit && charTyped == DELETE_KEY) {
            deletelastChar();
          }
        } else {
          inputLogic(charTyped);
        }
      }
    }
  }

 private:
  sf::Text textbox;
  std::ostringstream text;

  bool isSelected = false;
  bool hasLimit = false;
  int limit;

  void inputLogic(int charTyped) {
    if (charTyped != DELETE_KEY && charTyped != ENTER_KEY && charTyped != ESCAPE_KEY) {
      text << static_cast<char>(charTyped);
    } else if (charTyped == DELETE_KEY) {
      if (text.str().length() > 0) {
        deletelastChar();
      }
    }
    textbox.setString(text.str() + "_");
  }

  void deletelastChar() {
    std::string t = text.str();
    std::string newT = "";
    for (int i = 0; i < t.length() - 1; i++) {
      newT += t.at(i);
    }
    text.str("");
    text << newT;
    textbox.setString(text.str());
  }
};
