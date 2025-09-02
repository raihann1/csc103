#ifndef INTERACTABLEH
#define INTERACTABLEH

#include "Item.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Interactable {
public:
  Interactable();
  Interactable(string objName, string objDescription, string objLocation,
               string objItemRequired);
  string getName();
  string getDescription();
  string getLocation();
  string getObjItemRequired();
  void printInteractable();

private:
  string name;
  string description;
  string location;
  string itemNeeded;
};
#endif