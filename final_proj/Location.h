#ifndef LOCATIONH
#define LOCATIONH

#include "Interactable.h"
#include "Item.h"
#include <string>
#include <vector>
using namespace std;

class Location {
public:
  Location();
  Location(string name, string description);
  string getName();
  string getDescription();
  void addItem(Item item);
  void removeItem(Item item);
  void returnItem(Item item);
  void startGame();
  void lockLocation();
  void unlockLocation();
  bool getLocked();
  void removeInteractables(Interactable interactable);
  void addInteractable(Interactable interactable);
  int getInteractableSize();
  Interactable getInteractable(int index);
  int getItemSize();
  vector<Item> getItemsInLocation();
  vector<Interactable>getInteractablesInLocation();

private:
  string name;
  string description;
  vector<Item> itemsInLocation;
  vector<Interactable> interactablesInLocation;
  // bool isEnding;
  bool wonGame;
  bool locked;
};
#endif