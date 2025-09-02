#include <string>

#include "Item.h"
#include "iostream"
using namespace std;

Item::Item(string itemName, string itemDescription, string itemEmoji,
           string itemLocation) {
  name = itemName;
  description = itemDescription;
  emoji = itemEmoji;
  location = itemLocation;
}

string Item::getName() { return name; }

string Item::getDescription() { return description; }

string Item::getLocation() { return location; }

void Item::printItem() {
  cout << emoji << " " << name << " - " << description << endl;
}
