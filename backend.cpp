#include <stdlib.h>
#include <iostream>
#include <string>
#include <ctime>
#include <SFML/Window.hpp>
using std::cout;
using std::cin;
using std::string;

int main(){
  //set up SFML window

  //while the window is open, keep playing
  
  //start with inital coins
  //each game costs 10 coins
  int coins = 100;
  int command;

  //press play
  cout << "Press 1 to play" << '\n';
  cin >> command;
  //check for correct play command
  if (command != 1) {
    cout << "invalid" << '\n';
    return 0;
  }

  // Put in while loop, continues game while window is open
  bool play = true;
  while (play) {

    //check if they have enough coins 
    if (coins < 10) {
      cout << "invalid" << '\n';
      return 0;
    }
    //otherwise, play game
    int matches;
    int bet;
    matches = 0;

    cout << "How much would you like to bet?: " << "\n";
    cin >> bet;

    // if bets are invalid.
    while (bet > coins) {
      cout << "Invalid. Not enough coins. Current coin balance: " 
           << coins << "\n";
      cout << "Enter a new bet: "  << "\n";
      cin >> bet;
    }
    while (bet < 10) {
      cout << "Invalid. Minimum bet is 10." << "\n";
      cout << "Enter a new bet: "  << "\n";
      cin >> bet;
    }

    coins = coins - bet;

    //run the random number generator
    srand((unsigned) time(0));

    int num1 = rand() % 10 + 1;
    int num2 = rand() % 10 + 1;
    int num3 = rand() % 10 + 1;
    //check if there are matches

    cout << "num1: " << num1 << '\n';
    cout << "num2: " << num2 << '\n';
    cout << "num3: " << num3 << '\n';

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

  int replay;
  cout << "Your total coin value is now: " << coins << "\n";
  cout << "Play again? [0 for no, 1 for yes]" << "\n";
  cin >> replay;
  
  if (replay == 1) {
    play = true;
  } else {
    play = false;
  }

  }
  return 0;
}