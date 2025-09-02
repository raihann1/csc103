#include <string>
#include <vector>

#include "Location.h"
#include "iostream"
using namespace std;

Location::Location() {
  name = "null";
  description = "null";
}

Location::Location(string locationName, string locationDesc) {
  name = locationName;
  description = locationDesc;
  locked = false;
}

string Location::getName() { return name; }

string Location::getDescription() { return description; }

void Location::addItem(Item item) { itemsInLocation.push_back(item); }

void Location::removeItem(Item item) {
  /*
  The following method makes use of vector's erase() method.
  This method was found in Zybooks Section 7.13 Vector ADT.
  */
  for (int i = 0; i < itemsInLocation.size(); i++) {
    if (itemsInLocation.at(i).getName() == item.getName()) {
      itemsInLocation.erase(itemsInLocation.begin() + i);
      break;
    }
  }
}

vector<Item> Location::getItemsInLocation() { return itemsInLocation; }

void Location::lockLocation() { locked = true; }

void Location::unlockLocation() { locked = false; }

bool Location::getLocked() { return locked; }
// Used for both inserting and returning.
void Location::returnItem(Item item) { itemsInLocation.push_back(item); }

void Location::removeInteractables(Interactable interactable) {
  for (int i = 0; i < interactablesInLocation.size(); i++) {
    if (interactablesInLocation.at(i).getName() == interactable.getName()) {
      interactablesInLocation.erase(interactablesInLocation.begin() + i);
      break;
    }
  }
}

void Location::addInteractable(Interactable interactable) {
  interactablesInLocation.push_back(interactable);
}

void Location::startGame() {
  if (name != "X") {
    cout << name << endl;
    cout << description << endl;
    if (interactablesInLocation.size() > 0) {
      cout << "[Interactables] In this room, there are:" << endl;
    }
    for (int i = 0; i < interactablesInLocation.size(); i++) {
      interactablesInLocation.at(i).printInteractable();
    }
    if (itemsInLocation.size() > 0) {
      cout << endl << "[Items] In this room you find:" << endl;
    }
    for (int i = 0; i < itemsInLocation.size(); i++) {
      itemsInLocation.at(i).printItem();
    }
  } else {
    cout << "You have appeared to hit a wall... try navigating somewhere else."
         << endl;
  }
}

int Location::getInteractableSize() { return interactablesInLocation.size(); }

Interactable Location::getInteractable(int index) {
  return interactablesInLocation.at(index);
}

int Location::getItemSize() { return itemsInLocation.size(); }
vector<Interactable> Location::getInteractablesInLocation() {
  return interactablesInLocation;
}