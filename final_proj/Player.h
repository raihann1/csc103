#ifndef PLAYERH
#define PLAYERH

#include "Item.h"
#include <iostream>
#include <vector>
using namespace std;

class Player {
public:
  Player();
  void setPlayerName(string playerName);
  string getPlayerName();
  void setHouse(string ChosenHouse);
  string getChosenHouse();
  void setScore(int score);
  void incrementScore(int addScore);
  void decrementScore(int subtractScore);
  void printScore();
  void printInventory();
  void addItem(Item item);
  void removeItem(string item);
  string getItemName(int Num);
  int getInventorySize();
  vector<Item> getInventory();
  Item getItem(int Num);
  void checkItem(string item);

private:
  string name;
  int score;
  vector<Item> inventory;
  string House;
};

#endif