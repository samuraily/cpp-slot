#include <stdlib.h>
#include <iostream>
#include <string>
#include <ctime>
#include <SFML/Window.hpp>
using std::cout;
using std::cin;
using std::string;
using std::vector;

// Spin the slot machine
vector<int> spin(signed int coins, int bet) {
  int num1, num2, num3;

  //check if they have enough coins 
  if (coins < 0) {
    cout << "invalid" << '\n';
    return {0, 0, 0, 0};
  }
  //otherwise, play game
  int matches = 0;
  
  //run the random number generator
  srand((unsigned) time(0));

  num1 = rand() % 10 + 1;
  num2 = rand() % 10 + 1;
  num3 = rand() % 10 + 1;

  //check if there are matches
  if (num1 == num2)
    matches++;
  if (num1 == num3)
    matches++;
  if (num2 == num3)
    matches++;

  //check if there are 3 matches, add points
  if (matches == 1) {
    coins = coins + 2*bet;
  }
  if (matches == 3) {
    coins = coins + 10*bet;
  }

  vector<int> output = {coins, num1, num2, num3, matches};
  return output;
}