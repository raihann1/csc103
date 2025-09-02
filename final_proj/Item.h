#ifndef ITEMH
#define ITEMH

#include <string>
using namespace std;

class Item {
public:
  Item(string name, string description, string emoji, string location);
  string getName();
  string getDescription();
  string getLocation();
  void printItem();

private:
  string name;
  string description;
  string location;
  string emoji;
};
#endif