#include <iostream>
#include <vector>

#include "Item.h"
#include "Player.h"
using namespace std;

Player::Player() { name = "null"; }

void Player::setPlayerName(string playerName) { name = playerName; }

string Player::getPlayerName() { return name; }

void Player::setScore(int initscore) { score = initscore; }

void Player::incrementScore(int addscore) { score += addscore; }

void Player::decrementScore(int subscore) { score -= subscore; }

void Player::printScore() { cout << "🏆 Score: " << score << endl; }

void Player::setHouse(string ChosenHouse) { House = ChosenHouse; }

string Player::getChosenHouse() { return House; }

void Player::printInventory() {
  if (inventory.size() == 0) {
    cout << "You have no items in your inventory." << endl;
  }
  for (int i = 0; i < inventory.size(); i++) {
    cout << "Item " << i + 1 << ": ";
    inventory.at(i).printItem();
    cout << endl;
  }
}

void Player::addItem(Item item) { inventory.push_back(item); }

void Player::removeItem(string item) {
  for (int i = 0; i < inventory.size(); i++) {
    if (inventory.at(i).getName() == item) {
      inventory.erase(inventory.begin() + i);
      break;
    }
  }
}

vector<Item> Player::getInventory() { return inventory; }

string Player::getItemName(int Num) {
  string itemName = inventory.at(Num).getName();
  return itemName;
}

int Player::getInventorySize() { return inventory.size(); }

Item Player::getItem(int Num) { return inventory.at(Num); }

void Player::checkItem(string item) {
  bool found = false;
  for (int i = 0; i < inventory.size(); i++) {
    if (inventory.at(i).getName() == item) {
      cout << item << " ✅" << endl;
      found = true;
    }
  }
  if (!found) {
    cout << item << " ❌" << endl;
  }
}