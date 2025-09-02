#include <iostream>
#include <string>
#include <vector>

#include "Interactable.h"
#include "Item.h"
using namespace std;

Interactable::Interactable() {
  name = "null";
  description = "null";
  location = "null";
  itemNeeded = "null";
}
Interactable::Interactable(string objName, string objDescription,
                           string objLocation, string objItemRequired) {
  name = objName;
  description = objDescription;
  location = objLocation;
  itemNeeded = objItemRequired;
}
string Interactable::getName() { return name; }
string Interactable::getDescription() { return description; }
void Interactable::printInteractable() {
  cout << name << " - " << description << endl;
}
string Interactable::getObjItemRequired() { return itemNeeded; }
string Interactable::getLocation() { return location; }