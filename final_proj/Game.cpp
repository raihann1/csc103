/*
This is the main file that should be ran. Requires you to
compile all the files in the project. g++ *.cpp will do this
if ran outside of repl.it. Game seems to run smoother outside
repl.it in terms of printing text and viewing animations.
You may have to frequently scroll up/down when the console clears 
in repl.it during animations.

Repl.it takes anywhere from 0-15 seconds to compile and run. 
If nothing is being printed, it is probably because repl is
still loading. This issue does not occur when ran outside of repl.it.

This file makes use of the cin.ignore(), cin.get() & system("clear") commands. 
This was implemented by Raihan from prior knowledge.

Ascii art from https://emojicombos.com/

For best viewing experience, please make the console full-screen. Enjoy!
The game is designed to last 10-20 minutes.
*/

#include <unistd.h>

#include <fstream>
#include <iostream>

#include "Item.h"
#include "Location.h"
#include "Player.h"
using namespace std;
vector<string> parseItem(string line);
vector<string> parseInteractable(string line);
Location currentLocation;
vector<Item> items;
int currentRow = 0;
int currentCol = 0;
int currentTime;
void Reward(Interactable thing);
void action(string command);
void Startup();
void HelpMenu();
void PrintHelpMenu();
void startMiniGame();
void endGame();
void animateEnding();
void setSpawn(string selectedHouse);
void initializeGame();
void initializeMap();
string strToLower(string str);
void verifyMovementRow(int row);
void verifyMovementCol(int col);
void move(string input);
bool gameFinished = false;
string reader(string PlayerInput, vector<string> acceptedArguments,
              string repeatInput);
vector<vector<Location>> twoDMap;  // movement for map
int NorthorSouthMovement = 0;  // this is the movement for the player rows wise
int EastorWestMovement = 0;    // column wise
int EndGame = 0;
Player player;
int difference;
int main() {
  string input;
  Startup();
  while (EndGame == 0) {
    cin >> input;
    action(input);
    unsigned long int timeNow = time(0);
    difference = timeNow - currentTime;
  }
  if (EndGame == 1) {
    cout << "Dobby is sad. Dobby was unable to help " << player.getPlayerName() << " escape Hogwarts. Dobby hopes you had fun. Dobby will always be here to help. Dobby thanks you for playing."<< endl;
    player.printScore();
    unsigned long int timeNow = time(0);
    difference = (timeNow - currentTime) / 60;
    cout << "Your final time is: " << difference << " minute(s)" << endl;
  } else if(EndGame == 2) {
    endGame();
    player.printScore();
    unsigned long int timeNow = time(0);
    difference = (timeNow - currentTime) / 60;
    cout << "Your final time is: " << difference << " minutes" << endl;
  }
  return 0;
}

void Startup() {
  string name;
  string house;
  cout << "Loading game...." << endl;
  twoDMap.resize(4, vector<Location>(11));
  initializeMap();
  initializeGame();
  system("clear");
  cout << "Hello there! What is your name?" << endl;
  cin >> name;
  player.setPlayerName(name);
  cout << "⚡ 🤔 🧙 📚 🦅 🤓 👻 🕷️  🪄  📖 🦁 🏆" << endl;
  cout << "Welcome to The Hogwarts Treasure Hunt, " << name << "!" << endl
       << endl;
  cout << "My name is Dobby and I am a house-elf here at Hogwarts. "
          "During one of your hikes, you must have stumbled upon the Hogwarts School of "
          "Witchcraft and Wizardry. However, the school's barriers prevent you from "
          "leaving once you have entered. Now, you are tasked with finding "
          "treasure in order to begin the exit process. \n\nTo begin, we will "
          "have to start by disguising you as a typical student. Typically, the sorting hat "
          "will pick one of the four houses (Gryffindor, Ravenclaw, "
          "Hufflepuff, or Slytherin) for the students. However, since you are "
          "undercover, you can pick any house."
       << endl
       << endl;

  cout << "Choose one of the four houses to start in (only type the house "
          "name!):"
       << endl;
  cout << "Gryffindor 🦁" << endl;
  cout << "Ravenclaw 🦅" << endl;
  cout << "Hufflepuff 🦡" << endl;
  cout << "Slytherin 🐍" << endl;
  cin >> house;
  house = reader(strToLower(house),
                 {"slytherin", "gryffindor", "ravenclaw", "hufflepuff"},
                 "Invalid input! Please try again. Accepted arguments: "
                 "slytherin, gryffindor, ravenclaw, or hufflepuff.");
  system("clear");
  cout << "Great! Now that you have selected your house, allow me to explain "
          "the specifics of this treasure hunt. \n\nIn this school, there are "
          "26 different locations. In a few seconds, the sorting hat will "
          "teleport you into your house's dorm. Once you are in a location, "
          "you will get a quick description of the room along with "
          "items/interactables in the room. Some rooms will be locked. You'll "
          "have to find the key for that specific room in order to enter that "
          "room and obtain items. To win, you'll need to collect all of the "
          "items and meet in the Great Hall in order to form the Hogwarts emblem "
          "and exit. If you enter the "
          "Great Hall but do not have all the items, you will not be able to "
          "begin the formation process in order to exit."
       << endl;
  cout << endl
       << "In each location, the game will wait for your input. There are "
          "various commands you can use to navigate the game or to interact "
          "with items/interactables. Below you may find the list of "
          "commands!\n\n";
  HelpMenu();
  system("clear");
  setSpawn(house);
  player.setScore(0);
  currentTime = time(0);
}

void setSpawn(string selectedHouse) {
  player.setHouse(selectedHouse);
  int startingRow = 0;
  int startingCol = 0;
  int itemIndex;
  for (int i = 0; i < items.size(); i++){
    if (items.at(i).getName() == "Plate") {
      itemIndex = i;
    }
  }
  string playerHouse = player.getChosenHouse();
  if (strToLower(playerHouse) == "slytherin") {
    startingRow = 3;
    startingCol = 1;
    twoDMap.at(2).at(1).addItem(items.at(itemIndex));
  } else if (strToLower(playerHouse) == "gryffindor") {
    startingRow = 2;
    startingCol = 6;
    twoDMap.at(2).at(7).addItem(items.at(itemIndex));
  } else if (strToLower(playerHouse) == "ravenclaw") {
    startingRow = 3;
    startingCol = 9;
    twoDMap.at(2).at(9).addItem(items.at(itemIndex));
  } else if (strToLower(playerHouse) == "hufflepuff") {
    startingRow = 3;
    startingCol = 3;
    twoDMap.at(2).at(3).addItem(items.at(itemIndex));
  }
  twoDMap.at(startingRow).at(startingCol).startGame();
  currentRow = startingRow;
  currentCol = startingCol;
  currentLocation = twoDMap.at(currentRow).at(currentCol);
}
void HelpMenu() {
  cout << "Commands:" << endl;
  cout << "north, south, east, west - to move around the map" << endl;
  cout << "t - take item" << endl;
  cout << "i - inventory" << endl;
  cout << "u - interact with interactables" << endl;
  cout << "s - score" << endl;
  cout << "h - help (will display this menu)" << endl;
  cout << "o - objectives (displays items needed to win)" << endl;
  cout << "quit - quit the game" << endl;
  usleep(2500000);
  cout << "Press enter to get teleported into your house dorm!" << endl;
  cin.ignore();
  cin.get();
  currentLocation.startGame();
}
// does not accept input
void PrintHelpMenu() {
  cout << "Commands:" << endl;
  cout << "north, south, east, west - move" << endl;
  cout << "t - take item" << endl;
  cout << "i - inventory" << endl;
  cout << "u - interact with interactables" << endl;
  cout << "s - score" << endl;
  cout << "h - help (will display this menu)" << endl;
  cout << "o - objectives" << endl;
  cout << "quit - quit the game" << endl;
  cout << endl << endl;
  twoDMap.at(currentRow).at(currentCol).startGame();
}

void Reward(Interactable thing) {
  if (thing.getName() == "Door to Head of Slytherin Office") {
    cout << "You have opened the door to Slytherin's Head Office! You can now "
            "enter the room."
         << endl;
    twoDMap.at(1).at(0).unlockLocation();
  } else if (thing.getName() == "Door to Head of Hufflepuff Office") {
    cout << "You have opened the door to Hufflepuff's Head Office! You can now "
            "enter the room."
         << endl;
    twoDMap.at(2).at(4).unlockLocation();
  } else if (thing.getName() == "Door to Head of Gryffindor Office") {
    cout << "You have opened the door to Gryffindor's Head Office! You can now "
            "enter the room."
         << endl;
    twoDMap.at(3).at(7).unlockLocation();

  } else if (thing.getName() == "Door to Head of Ravenclaw Office") {
    cout << "You have opened the door to Ravenclaw's Head Office! You can now "
            "enter the room."
         << endl;
    twoDMap.at(1).at(10).unlockLocation();

  } else if (thing.getName() == "Door to the Staff Room") {
    cout << "You opened the Staff Room Door! You can now enter the room." << endl;
    twoDMap.at(2).at(5).unlockLocation();

  } else if (thing.getName() == "Spot for missing Tapestry") {
    for (int l = 0; l < items.size(); l++) {
      if (strToLower(items.at(l).getName()) == "key to the staff room") {
        player.addItem(items.at(l));
        cout << "As you set up the Tapestry, what appears to be a key drops out of the folded up tapestry and you decide to pick it up. " << "You found the " << items.at(l).getName() << "!" << endl;
      }
    }
  } else if (thing.getName() == "Dobby") {
    for (int l = 0; l < items.size(); l++){
      if (strToLower(items.at(l).getName()) == "bottle of unknown dark liquid"){
        player.addItem(items.at(l));
        cout << "Dobby receives Plate. Dobby gives you a " << items.at(l).getName() << ". Dobby thinks you may find a use for this somewhere." << endl;
      }
    }
  } else if (thing.getName() == "Magical Device") {
    for (int l = 0; l < items.size(); l++){
      if (strToLower(items.at(l).getName()) == "lost book"){ 
        player.addItem(items.at(l));
        cout << "For some reason the Magical Device spat out a " << items.at(l).getName() << ". You decide to take it with you." << endl;
      }
    }
  } else if (thing.getName() == "Bookshelf") {
    for (int l = 0; l < items.size(); l++) {
      if (strToLower(items.at(l).getName()) == "fallen tapestry"){
        player.addItem(items.at(l));
        cout << "It appears a book was missing from the bookshelf. After placing the book in the correct spot on the bookshelf, a " << items.at(l).getName() << " fell on your head. You pick it off the ground while holding your head wondering how that happened. You now have the " << items.at(l).getName() << endl;
    }
  }
}
}
void action(string command) {
  command = reader(
      command,
      {"north", "south", "west", "east", "t", "quit", "s", "i", "u", "h", "o"},
      "Invalid input! Try again. If you need to see the command list, type 'h'.");
  if (strToLower(command) == "t") {
    vector<Item> itemsInLoc =
        twoDMap.at(currentRow).at(currentCol).getItemsInLocation();
    if (itemsInLoc.size() == 0) {
      cout << "There are no items in this location!" << endl;
    } else {
      for (int i = 0; i < itemsInLoc.size(); i++) {
        player.addItem(itemsInLoc.at(i));
        cout << "You got a(n) " << itemsInLoc.at(i).getName() << "!" << endl;
        twoDMap.at(currentRow).at(currentCol).removeItem(itemsInLoc.at(i));
        player.incrementScore(25);
      }
    }
  } else if (strToLower(command) == "i") {
    player.printInventory();
  } else if (strToLower(command) == "u") {
    vector<Interactable> interactablesInLoc =
        twoDMap.at(currentRow).at(currentCol).getInteractablesInLocation();
    if (interactablesInLoc.size() == 0) {
      cout << "There is nothing to interact with in this location!" << endl;
    } else {
      bool interacted = false;
      for (int i = 0; i < interactablesInLoc.size(); i++) {
        for (int j = 0; j < player.getInventorySize(); j++) {
          if (interactablesInLoc.at(i).getObjItemRequired() ==
              player.getItem(j).getName()) {
            player.removeItem(player.getItem(j).getName());

            Reward(interactablesInLoc.at(i));
            twoDMap.at(currentRow)
                .at(currentCol)
                .removeInteractables(interactablesInLoc.at(i));
            interacted = true;
          }
        }
      }
      if (!interacted) {
        cout << "You do not have the item required to interact with this "
                "Interactable!";
        cout << "\n\n";
        twoDMap.at(currentRow).at(currentCol).startGame();
      }
    }
  } else if (strToLower(command) == "quit") {
    EndGame = 1;
  } else if (strToLower(command) == "s") {
    player.printScore();
  } else if (strToLower(command) == "h") {
    PrintHelpMenu();
  } else if (strToLower(command) == "o") {
    string displayItems;
    cout << "Here are the items you need to collect in order to "
    " be able to exit the school: " << endl;
    player.checkItem("Snake Piece");
    player.checkItem("Green Piece");
    player.checkItem("Honey Badger Piece");
    player.checkItem("Gold Piece");
    player.checkItem("Lion Piece");
    player.checkItem("Red Piece");
    player.checkItem("Raven Piece");
    player.checkItem("Blue Piece");
    player.checkItem("Metal Piece");
    player.checkItem("Patterned Piece");
  } else {
    cout << "\n\n";
    move(command);
    Location currentLocation = twoDMap.at(currentRow).at(currentCol);
    if (strToLower(currentLocation.getName()) == "potion classroom" &&
        !gameFinished) {
      // make it so minigame only runs once
      startMiniGame();
    } else if (strToLower(currentLocation.getName()) == "the great hall") {
      vector<Item> playerInventory = player.getInventory();
      vector<string> itemsToWin;
        itemsToWin = {"snake piece", "green piece", "honey badger piece",
                      "gold piece", "lion piece", "red piece", "raven piece", "blue piece", 
                     "metal piece", "patterned piece"};
      int numItemsFound = 0;
      for (int i = 0; i < playerInventory.size(); i++) {
        for (int j = 0; j < itemsToWin.size(); j++) {
          if (strToLower(playerInventory.at(i).getName()) == itemsToWin.at(j)) {
            numItemsFound++;
            itemsToWin.erase(itemsToWin.begin() + j);
          }
        }
      }
      if (numItemsFound == 10) {
        EndGame = 2;
      } else {
        currentLocation.startGame();
        cout << "You didn't find all the pieces yet! Keep looking!" << endl;
      }
    } else {
      currentLocation.startGame();
    }
  }
}

vector<string> parseItem(string line) {
  string text = line;
  vector<string> item;
  string itemName = "";
  string itemDesc = "";
  string itemEmoji = "";
  string itemLoc = "";
  int index = text.find(':');
  itemName = text.substr(0, index);
  text = text.substr(index + 1, line.size() - index);
  index = text.find(':');
  itemEmoji = text.substr(0, index);
  text = text.substr(index + 1, line.size() - index);
  index = text.find(':');
  itemDesc = text.substr(0, index);
  text = text.substr(index + 1, line.size() - index);
  itemLoc = text;
  item.push_back(itemName);
  item.push_back(itemDesc);
  item.push_back(itemEmoji);
  item.push_back(itemLoc);
  return item;
}
vector<string> parseInteractable(string line) {
  string text = line;
  vector<string> interactable;
  string interactableName = "";
  string interactableDesc = "";
  string interactableLoc = "";
  string interactableItem = "";
  int index = text.find(':');
  interactableName = text.substr(0, index);
  text = text.substr(index + 1, line.size() - index);
  index = text.find(':');
  interactableDesc = text.substr(0, index);
  text = text.substr(index + 1, line.size() - index);
  index = text.find(':');
  interactableLoc = text.substr(0, index);
  text = text.substr(index + 1, line.size() - index);
  index = text.find(':');
  interactableItem = text.substr(0, index);
  interactable.push_back(interactableName);
  interactable.push_back(interactableDesc);
  interactable.push_back(interactableLoc);
  interactable.push_back(interactableItem);
  return interactable;
}

/* This function makes use of ifstream in order to
parse information from text files. This was implemented from prior knowledge.
*/
void initializeGame() {
  ifstream itemsFile("Item.txt");
  ifstream KeywordsFile("Keywords.txt");
  ifstream interactablesFile("Interactable.txt");
  string location;
  string locName;
  string locDesc;
  string item;
  string itemName;
  string itemLoc;
  string itemDesc;
  string commands;
  string interactable;
  string interactName;
  string interactDesc;
  string interactLocation;
  vector<Interactable> interactables;
  vector<Location> locations;
  int numInteractions = 0;
  int numLocations = 0;
  int numItems = 0;

  while (getline(interactablesFile, interactable)) {
    vector<string> interactableInfo = parseInteractable(interactable);
    numInteractions++;
    interactables.push_back(
        Interactable(interactableInfo.at(0), interactableInfo.at(1),
                     interactableInfo.at(2), interactableInfo.at(3)));
  }
  while (getline(itemsFile, item)) {
    vector<string> itemInfo = parseItem(item);
    numItems++;
    items.push_back(
        Item(itemInfo.at(0), itemInfo.at(1), itemInfo.at(2), itemInfo.at(3)));
  }

  for (int i = 0; i < twoDMap.size(); i++) {
    for (int j = 0; j < twoDMap.at(i).size(); j++) {
      for (int k = 0; k < items.size(); k++) {
        if (strToLower(items.at(k).getLocation()) ==
            strToLower(twoDMap.at(i).at(j).getName())) {
          twoDMap.at(i).at(j).returnItem(items.at(k));
        }
      }
    }
  }
  for (int i = 0; i < twoDMap.size(); i++) {
    for (int j = 0; j < twoDMap.at(i).size(); j++) {
      for (int k = 0; k < interactables.size(); k++) {
        if (strToLower(interactables.at(k).getLocation()) ==
            strToLower(twoDMap.at(i).at(j).getName())) {
          twoDMap.at(i).at(j).addInteractable(interactables.at(k));
        }
      }
    }
  }
}

string reader(string PlayerInput, vector<string> acceptedArguments,
              string repeatInput) {
  string returnInput = PlayerInput;
  PlayerInput = strToLower(PlayerInput);
  bool inVector = false;
  for (int i = 0; i < acceptedArguments.size(); i++) {
    if (strToLower(acceptedArguments.at(i)) == PlayerInput) {
      inVector = true;
    }
  }
  string newInput;
  while (!inVector) {
    cout << repeatInput << endl;
    cin >> newInput;
    for (int i = 0; i < acceptedArguments.size(); i++) {
      if (strToLower(acceptedArguments.at(i)) == strToLower(newInput)) {
        inVector = true;
        returnInput = newInput;
      }
    }
  }
  return returnInput;
}


void move(string input) {
  input = strToLower(input);

  if (input == "east") {
    verifyMovementCol(currentCol + 1);
  } else if (input == "west") {
    verifyMovementCol(currentCol - 1);
  } else if (input == "north") {
    verifyMovementRow(currentRow - 1);
  } else if (input == "south") {
    verifyMovementRow(currentRow + 1);
  }
}
void verifyMovementRow(int row) {
  if ((row < 0) || (row > 3)) {
    cout << "You have appeared to hit a wall... try navigating somewhere else." << endl;
    cout << "\n\n";
  } else if (twoDMap.at(row).at(currentCol).getName() == "X") {
    cout << "You have appeared to hit a wall... try navigating somewhere else." << endl;
    cout << "\n\n";
  } else if ((row == 2 && currentCol == 4) || (row == 1 && currentCol == 4) ||
             (row == 1 && currentCol == 6) || (row == 2 && currentCol == 6)) {
    cout << "You have appeared to hit a wall... try navigating somewhere else." << endl;
    cout << "\n\n";
  } else if (twoDMap.at(row).at(currentCol).getLocked()) {
    cout << twoDMap.at(row).at(currentCol).getName() << endl;
    cout
        << "It appears this location is locked... Perhaps a key is required to "
           "enter this room! Teleporting you back to your previous location..."
        << endl;
    cout << "\n\n";
  } else {
    currentRow = row;
  }
}

void verifyMovementCol(int col) {
  if ((col < 0) || (col > 10)) {
    cout << "You have appeared to hit a wall... try navigating somewhere else." << endl << endl;
  } else if (twoDMap.at(currentRow).at(col).getName() == "X") {
    cout << "You have appeared to hit a wall... try navigating somewhere else." << endl << endl;
  } else if ((currentRow == 2 && currentCol == 5 && col == 4) ||
             (currentRow == 2 && currentCol == 5 && col == 6) ||
             currentRow == 2 && col == 5) {
    cout << "You have appeared to hit a wall... try navigating somewhere else." << endl << endl;
  } else if (twoDMap.at(currentRow).at(col).getLocked()) {
    cout << twoDMap.at(currentRow).at(col).getName() << endl;
    cout << "It appears this location is locked... Perhaps a key is required "
            "to enter this room!"
         << endl;
  } else {
    currentCol = col;
  }
}
string strToLower(string str) {
  string newStr = "";
  for (int i = 0; i < str.size(); i++) {
    newStr += tolower(str.at(i));
  }
  return newStr;
}

void initializeMap() {
  vector<string> row0 = {
      "X:Dead end.",
      "Potion Classroom:A chilly room with jarred animal parts lining the "
      "walls.",
      "X:Dead end.",
      "Kitchen:The smell of food and heat of the oven warms you ever so "
      "slightly.",
      "X:Dead end.",
      "The Great Hall:The main hall for any gatherings and for when you need "
      "some food.",
      "X:Dead end.",
      "Hospital Wing:The injured may come here to heal and rest.",
      "X:Dead end.",
      "Library:A grand space with many books are here that cover all types of "
      "topics in magic and maybe more.",
      "X:Dead end."};
  vector<string> row1 = {
      "The Head of Slytherin's Office:A gloomy and dimmly lit room with "
      "shelves full of jars with animal and plant parts.",
      "Dungeons:The cold temperture of the dungeons send shivers down your spine.",
      "Grand Staircase (Dungeon):The staircase that connects the dungeons and "
      "basement.",
      "Basement:Not very remarkable outside the faint smell of food, there might be a kitchen nearby.",
      "Grand Staircase (Basement):A staircase which connects the basement to "
      "the first floor.",
      "The Entrance Hall:The main entrance to Hogwarts Castle and north of here is the Great Hall.",
      "Grand Staircase (First Floor):A connecting set of stairs from the "
      "ground floor to the second floor.",
      "Tapestry Corridor:A hallway lined with beautiful tapestry.",
      "Grand Staircase (Ravenclaw Tower)  :The stairs which takes you up to "
      "the Ravenclaw tower.",
      "Ravenclaw Tower:The tower is quite dim and moody with a cold blue light "
      "illuminating the tower.",
      "The Head of Ravenclaw's Office:The Office of Ravenclaw's headmaster."};
  vector<string> row2 = {
      "X:Dead end.",
      "Slytherin Common Room:A room where the symbol for Slytherins was "
      "adorned on the chairs and the wall with a dim green light illuminating "
      "the room.",
      "X:Dead end.",
      "Hufflepuff Common Room:A room with a very earthy warm color palette and "
      "warm light which seems to be comming from the ceiling.",
      "The Head of Hufflepuff's Office:This room is for the Head of "
      "Hufflepuff.",
      "Staff Room:You've finally unlocked the hardest room in this place. They sure do have some tight security measures around here.",
      "Gryffindor Dormitories:The dorms for Gryffindor students.",
      "Gryffindor Common Room:A stoney room with an accent of red and sunlight "
      "shining through the windows.",
      "X:Dead end.",
      "Ravenclaw Common Room:The room is decorated with the color blue and the "
      "image of Ravenclaw.",
      "X:Dead end."};
  vector<string> row3 = {
      "X:Dead end.",
      "Slytherin Dormitories:The dorms for the Slytherin students.",
      "X:Dead end.",
      "Hufflepuff Dormitories:The dorms for Hufflepuff students.",
      "X:Dead end.",
      "X:Dead end.",
      "X:Dead end.",
      "The Head of Gryffindor's Office:The head of Gyffindor resides in this "
      "room.",
      "X:Dead end.",
      "Ravenclaw Dormitories:The dorm for Ravenclaw students.",
      "X:Dead end."};
  for (int i = 0; i < twoDMap.size(); i++) {
    for (int j = 0; j < twoDMap.at(i).size(); j++) {
      if (i == 0) {
        string locationName;
        string locationDesc;
        for (int i = 0; i < row0.at(j).size(); i++) {
          string location = row0.at(j);
          if (row0.at(j).at(i) == ':') {
            locationName = location.substr(0, i);
            locationDesc = location.substr(i + 1, location.size() - i);
            break;
          }
        }
        twoDMap.at(i).at(j) = Location(locationName, locationDesc);
      } else if (i == 1) {
        string locationName;
        string locationDesc;
        for (int i = 0; i < row1.at(j).size(); i++) {
          string location = row1.at(j);
          if (row1.at(j).at(i) == ':') {
            locationName = location.substr(0, i);
            locationDesc = location.substr(i + 1, location.size() - i);
            break;
          }
        }
        twoDMap.at(i).at(j) = Location(locationName, locationDesc);
        if (locationName == "The Head of Slytherin's Office") {
          twoDMap.at(i).at(j).lockLocation();
        } else if (locationName == "The Head of Ravenclaw's Office") {
          twoDMap.at(i).at(j).lockLocation();
        }
      } else if (i == 2) {
        string locationName;
        string locationDesc;
        for (int i = 0; i < row2.at(j).size(); i++) {
          string location = row2.at(j);
          if (row2.at(j).at(i) == ':') {
            locationName = location.substr(0, i);
            locationDesc = location.substr(i + 1, location.size() - i);
            break;
          }
        }
        twoDMap.at(i).at(j) = Location(locationName, locationDesc);
        if (locationName == "The Head of Hufflepuff's Office") {
          twoDMap.at(i).at(j).lockLocation();
        } else if (locationName == "Staff Room") {
          twoDMap.at(i).at(j).lockLocation();
        }
      } else if (i == 3) {
        string locationName;
        string locationDesc;
        for (int i = 0; i < row3.at(j).size(); i++) {
          string location = row3.at(j);
          if (row3.at(j).at(i) == ':') {
            locationName = location.substr(0, i);
            locationDesc = location.substr(i + 1, location.size() - i);
            break;
          }
        }
        twoDMap.at(i).at(j) = Location(locationName, locationDesc);
        if (locationName == "The Head of Gryffindor's Office") {
          twoDMap.at(i).at(j).lockLocation();
        }
      }
    }
  }
}

void startMiniGame() {
  // // Lock potion classroom
  // twoDMap.at(1).at(0).lockLocation();
  cout
      << "Uh-oh! You have stumbled upon a Professor outside the Potions classroom.\n\n"
         "Welcome to the Spell Memory challenege!\n\n"
         "My name is Professor Filtwick, the Charms professor here at Hogwarts."
         "\n\nI see that you are attempting to gain access to my classroom."
         "\nHowever, I will not unlock it that easily! You must beat this "
         "spell memory challenge in order to obtain access to this room."
         "\n\nGame Instructions: I will show you anywhere from two to three "
         "spells for a few seconds. "
         "Then, you will need to repeat the spells you saw on the screen in "
         "order to move onto the next round. "
         "There will be a total of 5 rounds. You must win 4 out of 5 rounds to "
         "proceed. \n\nImportant note: Do NOT type anything while you are "
         "observing the spells. If you do, the game will not let you input the "
         "spells you saw. Also, you may need to scroll up when you type yes as the "
         "console clears and the spells show up at the top.\n\n"
      << endl;
  cout << "\nI will begin the game once you type yes. Please scroll up as soon as you say yes so you can see the output. To stop, please type no." << endl;
  string input;
  cin.ignore();
  getline(cin, input);
  srand(time(0));
  if (strToLower(input) == "yes") {
    int roundNum = 0;
    int roundsWon = 0;
    usleep(1000000);
    while (roundNum < 5) {
      vector<string> spells = {"avada kedavra", "expelliarmus", "stupefy",
                               "accio", "expulso", "aguamenti", "aberto",
                               "apparate", "wingardium leviosa", "reparo",
                               "protego", "lumos", "nox"};
      string display = "";
      vector<string> displayItems;
      vector<string> userEntered;
      int numDisplay = 2;
      if (roundNum > 3) numDisplay = 3;
      for (int i = 0; i < numDisplay; i++) {
        int index = rand() % spells.size();
        string randSpell = spells.at(index);
        display += randSpell + "\n";
        displayItems.push_back(randSpell);
        spells.erase(spells.begin() + index);
      }
      system("clear");
      cout << "Remember, you must spell it correctly and include any proper "
              "spacing! Do NOT type anything at this point, simply observe. "
              "The spells are:\n\n"
           << display << endl;
      usleep((numDisplay + 2) * 1000000);
      system("clear");

      cout << "Enter the spells you saw (one per line). Once you entered one "
              "spell, click enter to continue entering spells! The program "
              "will stop accepting input after you've entered the possible "
              "number of spells."
           << endl;
      string userinput;
      int numInputs = 0;
      while (numInputs < numDisplay) {
        getline(cin, userinput);
        userEntered.push_back(userinput);
        numInputs++;
      }
      int numFound = 0;
      for (int i = 0; i < userEntered.size(); i++) {
        for (int j = 0; j < displayItems.size(); j++) {
          if (strToLower(userEntered.at(i)) == displayItems.at(j)) {
            numFound++;
            displayItems.erase(displayItems.begin() + j);
          }
        }
      }
      if (numFound != numDisplay) {
        cout << "You did not enter all the spells correctly for this round. "
                "Continuing..."
             << endl;
        usleep(2000000);
      } else {
        cout << "You entered all the spells correctly for this round! "
                "Continuing..."
             << endl;
        usleep(2000000);
        roundsWon++;
      }
      roundNum++;
    }
    if (roundsWon < 4) {
      cout << "\n\nYou did not win the spell memory challenge... Teleporting you "
              "back to the Dungeons."
           << endl;
      cout << "You should study up on the material if you want to be able to defeat me. You think that there should be a library around here somewhere." << endl;
      action("south");
    } else {
      cout << "\n\nYou won the spell memory challenge! You have been granted "
              "access to the potion classroom!\n"
           << endl;
      gameFinished = true;
      twoDMap.at(0).at(1).startGame();
    }
  } else if (strToLower(input) == "no") {
    cout << "You have chosen to not play the game. Teleporting you back to the "
            "Dungeons.\n"
         << endl;
    action("south");
  } else {
    cout << "Did not get yes or no response. Teleporting you back to the "
            "Dungeons.\n"
         << endl;
    action("south");
  }
}
/*
This method makes use of the raw string literal in order to
print ASCII art. This method was adapted from a Stackoverflow
post: https://stackoverflow.com/questions/37765925/ascii-art-in-c
*/
  void endGame() {
    system("clear");
    cout << "Congratulations! It seems like you have all the pieces to form the Hogwarts emblem. Starting formation proces.... (it is recommended to make your console bigger to view the ascii art properly)" << endl;
    usleep(3000000);
    system("clear");
    cout << "Building Slytherin insignia..." << endl;
    usleep(1500000);
    system("clear");
    cout << R"(
      ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣤⣄⣶⣶⣶⣶⣾⣿⣿⣿⣶⣶⣶⣦⣄⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⢰⣦⣄⣀⠀⠀⠀⠀⣀⣀⡀⠀⢀⣀⣤⣤⣴⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⡿⠿⣟⠻⠏⢩⢹⣿⣿⣿⣿⡿⠿⢿⣿⡿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⢀⣘⠻⠯⢑⣊⣉⣀⠀⠀⢸⡘⠛⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⡆⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠙⠛⠛⠿⢿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⡏⠀⣠⡾⠛⠻⣿⡿⠟⠃⠸⡇⢀⣴⣷⠀⠀⠀⠀⠀⠀⢀⣴⣧⡀⢹⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠻⢧⡀⠀⠀⠀⠀⠀
      ⠀⠀⡀⠄⠒⡇⠀⣿⣦⡀⠀⠉⠀⠀⠀⠀⡇⠀⣿⣿⠀⠀⣀⠀⠀⣀⢨⣿⡏⠀⢸⣿⣧⣴⣦⡀⠀⣴⣶⣶⣄⠀⣀⣴⣤⣤⡀⠀⣿⠇⠀⠀⠀⠀⠈⠧⠤⢄⡀⠀
      ⣠⠊⠀⠀⠀⠃⠀⠙⣿⣿⣶⣄⠀⠀⠀⠀⡇⠀⣿⣿⠘⣿⣿⠀⣿⡇⢸⣿⡇⠀⢸⣿⠃⢹⣿⡇⢸⣿⠁⣸⠿⠀⣿⣿⠛⠟⠁⣤⣴⡆⢀⣀⣠⡀⠀⠀⠀⠀⠉⡦
      ⠈⠢⣤⠀⠀⢸⠀⠀⠈⠻⣿⣿⣿⣦⡀⠀⠇⠀⣿⣿⠀⠸⣿⣦⣿⠁⠘⣿⣇⠄⣸⣿⡀⢸⣿⡇⢸⣿⡋⢀⡆⠀⣿⣿⠀⠀⢀⣿⡿⠀⢸⣿⡿⢻⣿⡆⠀⢀⠞⠁
      ⠀⠀⠀⠰⡀⣸⣤⣤⣤⣄⠈⠻⣿⣿⣧⠀⠀⠀⠾⠿⠂⠀⢹⡿⠃⠀⠈⠛⠁⠈⠉⠁⠀⠚⠛⠓⠈⠛⠷⠛⠁⠠⠿⠿⠄⠀⣼⣿⡇⠀⣸⣿⠁⢸⣿⠇⢠⠋⠀⠀
      ⠀⠀⠀⠀⣿⣿⠋⠁⠉⢿⣷⠀⠸⣿⣿⠀⢠⠀⠀⣴⣷⡾⠟⠁⣀⡤⠔⠒⠉⠉⠉⠉⠉⠓⠢⢄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠁⠠⠿⠇⢀⣾⣿⠀⢳⠀⠀⠀
      ⠀⠀⠀⢸⣿⠁⠀⢦⣤⣾⡿⠀⢠⣿⣿⠂⢸⠀⣀⠠⣄⣒⢉⡿⠋⠀⠀⠀⢰⣄⣠⣀⣄⢀⠀⠀⠈⠣⣉⣲⢶⣤⣤⣀⣀⣀⠀⠀⠀⠀⠀⠀⠉⠙⠃⢀⡏⠀⠀⠀
      ⠀⠀⠀⢹⣿⣆⠀⠀⠀⠀⠀⢀⣾⣿⠟⠀⠸⠉⢰⣿⣿⢏⡞⠀⠀⠀⣠⣾⢟⣻⣯⣭⣟⡻⢿⣄⠀⠀⢶⣶⣍⠚⠋⠉⢽⢻⣷⣦⣄⡀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀
      ⠀⠀⠀⠀⠻⣿⣿⣶⣦⣴⡶⠟⠋⣁⣀⡤⠃⢀⣼⣿⡟⣼⡃⠀⠀⢰⡟⠀⠈⢻⡟⠁⠈⢻⣇⢿⡇⠀⣠⣤⠔⠒⠤⡂⢌⣿⣿⣿⣿⣿⣷⣦⣄⡀⠀⡜⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⡇⠀⠀⣀⠤⣒⣭⡍⢰⣾⣿⣿⣿⣿⣿⡇⡟⡁⠀⠀⢸⡇⣄⠰⣿⣿⠆⣴⣿⣿⣮⡳⡀⠹⡇⠐⠢⡔⣿⣿⣿⣿⡿⢿⣿⣿⣿⡇⠉⠙⠃⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠧⠒⠉⣶⣿⣿⣿⡇⠈⢿⣿⣿⣿⠿⢻⣇⢿⣷⠀⠀⠀⠙⢎⡳⣌⣡⣾⣿⣿⣿⣿⡇⠀⣥⣢⠅⣒⠛⣢⣽⡿⠋⠀⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⠀⣤⡙⣿⢱⢹⢸⣿⣌⢿⣷⣀⠀⠀⠀⠙⠢⢝⡻⢿⣿⣿⣿⣷⣠⣿⣿⣿⣶⣶⣿⣫⣶⣄⠀⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⢸⣿⣿⡌⢸⠀⣞⢿⣿⣷⣝⠿⣷⣦⣄⣀⡀⠀⠉⠐⠨⢝⡻⢿⣿⣿⣿⣿⣿⠟⠹⣿⣿⣿⠆⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⠀⢹⡟⡱⠟⣇⠘⢷⡒⢭⡻⣷⣮⣝⡻⣿⣿⣦⣀⠀⠀⠀⠉⠒⢭⡻⠿⢟⣅⠀⠀⣹⣿⡁⠀⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⡇⢠⡿⡸⠁⢸⡾⢦⡀⠳⡀⠹⡽⣿⣿⣿⣷⠍⠻⢿⣿⣶⣤⣀⠀⠀⠈⢳⣝⢿⡆⢴⣿⣿⣷⡄⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⡇⠸⡇⡇⠀⢾⡀⣀⡷⠰⣧⠀⡇⣹⣿⣿⣿⣄⣠⣶⣭⡻⢿⣿⣷⠄⠀⠈⠻⢧⡀⠈⢻⡿⠋⠀⣼⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⡇⢀⣷⢻⣄⠀⠉⠉⠀⢀⡿⢠⡇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣝⢿⣷⣄⠀⠀⠀⢻⣀⣾⣷⣄⠀⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⡇⠘⣿⣷⢉⣓⠶⠾⣴⣾⠃⢸⠇⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⠹⣿⣦⠀⠀⢈⣇⣿⣿⡿⠁⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⡇⠀⢈⣴⣿⣿⣿⣿⢹⣿⠀⢿⢠⣾⣿⣿⣿⣿⡿⣿⣿⣿⣿⣷⣄⣤⢻⣿⠀⠀⠸⣿⣹⣯⠀⠀⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⢰⣿⣿⣿⣿⡿⠿⢸⣿⠀⢸⣼⣿⣿⣿⠿⣫⣾⣮⠻⣿⣿⣿⣿⡿⣼⣿⠀⢠⣀⡇⣿⣿⣷⡄⣿⣿⣿⣿⡁⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⢸⣿⣿⣿⣿⡅⢀⢸⣿⠀⠸⣷⡻⠟⠁⠀⠹⣿⠋⠀⠈⠻⣿⠟⣱⣿⠏⢀⣿⣿⠁⣿⣿⣿⡇⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⢸⣿⣿⣿⣿⣿⣿⡜⣿⣧⡀⠉⢹⢤⡀⠀⣴⣿⣦⠀⠀⣀⣴⡾⠟⠀⢀⣾⣿⣇⣰⣿⣿⣿⡇⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣷⡘⠻⢿⣿⣿⣿⣿⣿⣼⣿⣿⣶⡀⠀⠉⠛⠒⠒⠶⠟⠛⠋⠉⢀⣠⣶⣿⣿⣿⣿⣿⡿⢟⣫⣵⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣷⣦⣈⠙⠻⠿⠟⣱⣦⣍⡻⢿⣿⣿⣿⣆⣠⣶⣦⣤⣴⡾⠿⠛⣋⡻⣿⠿⣋⣥⣾⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⠙⠻⢿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣀⠙⠻⠿⠛⢷⣶⣬⡭⣭⣭⣭⡭⣥⣴⣶⠊⠻⢟⣫⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠋⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠛⠻⢿⣿⣿⣿⣿⣿⣿⣶⣤⣀⠈⠻⡏⠀⢘⣿⣟⠀⠈⡿⣋⣤⣶⣿⣿⣿⣿⣿⣿⣿⠿⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⡚⠿⢟⣠⣴⣾⣿⣿⣿⣿⣿⣿⣿⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠿⣿⡿⠟⠋⠁⠀⠀⠀⠀⠀⠀		
      )" << '\n';
      cout << "\n\n";
      cout << "Slytherin insignia successfuly built. Continuing formation process..." << endl;
      usleep(2500000);
      system("clear");
      cout << "Building Gryffindor insignia..." << endl;
      usleep(1500000);
      system("clear");
      cout << R"(
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⣤⣤⣶⣶⣶⣶⣶⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣶⣶⣶⣶⣦⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⡟⠛⠋⠉⠉⣹⡍⠁⠀⠀⠀⢠⣿⡇⣶⣶⣶⣶⣾⣿⣿⣿⣭⣽⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣷⡄⠀⠀⢰⣿⣿⠄⠀⠤⣤⣭⣟⣡⣽⣻⣿⣿⣿⣿⡟⠈⣿⣿⣿⢹⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣄⢠⣿⢿⣋⣾⣿⣿⣿⣿⣿⣿⠛⠿⠊⠻⢿⣿⣿⣶⣿⣿⣿⣏⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⠃⠀⠵⣿⣿⣿⣿⣿⣿⠿⠓⠀⠀⠀⠤⣤⠄⠉⠛⢻⣿⣿⣿⣿⡜⣿⣿⣿⣿⣦⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⣠⣦⣤⣤⣴⣾⣿⣿⣿⣿⣿⣿⠃⠀⠀⣾⣿⣿⣿⣿⣿⠠⠶⠆⠀⠀⠀⠀⠀⠀⠀⠀⠉⣿⣿⣿⣿⣿⣜⢿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠈⠃⠀⠀⠼⣿⣿⣿⣿⣿⣿⣦⣄⣀⠀⠀⠀⠀⣴⣷⣶⣶⣴⣿⣿⣿⣿⣿⣿⠗⠀⠀⠈⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⢸⣿⣿⣿⣿⡿⢿⡟⠋⠀⠀⠀⣴⡀⢀⠾⣻⣿⣿⣿⣿⣿⣿⣿⣷⡆⠀⠀⠀⠻⠿⠿⠿⣿⣏⠀⣹⣿⣿⠇⠀⠀⠀⢰⣶⡎⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⢸⣿⣿⣿⣿⠀⣾⣿⣄⠀⠀⣼⣿⣿⢄⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⣿⣿⣷⣿⣿⣿⡄⠀⠀⠀⣿⣿⡇⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⢸⣿⣿⣿⣿⣾⣿⣿⣿⠆⠸⣿⣏⡵⠿⡻⣿⡿⣻⣿⣿⣿⣿⠋⢿⣿⣿⣷⣶⣶⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⣰⣿⣿⡇⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠸⣿⣿⣿⣿⡏⣿⣿⡏⠀⠀⠙⣿⡿⠃⠀⠘⢱⣿⣿⣿⣿⠃⠀⠀⠻⢿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⠀⢠⣿⣿⣿⣿⡇⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⣿⣿⣿⠟⠉⠈⠙⠢⣄⠀⠀⠈⠁⠀⠀⠀⠘⢸⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀⠙⠻⠿⠿⢿⣿⡿⠃⠀⠀⠀⣸⣿⣿⢿⣿⡇⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⣿⣿⠃⠀⠀⢀⣤⠒⢄⠑⢄⣼⣦⠀⠀⠀⢠⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⡇⠀⠀⠀⢀⣿⣿⡁⢈⣿⡇⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⢹⣿⡄⠀⣾⢁⣾⣧⡀⠱⡀⠹⣿⣧⡀⢠⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠑⠤⢄⣀⣀⡀⢹⠀⠀⠀⣼⣿⣿⣷⣾⣿⢱⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⢸⣿⣿⣦⣽⣾⣿⣿⡗⠀⣷⠀⢹⡿⠁⠸⣿⣿⣿⣧⠀⠀⡠⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠒⠚⠛⠛⠛⠻⠿⠿⢸⣿⣿⣿⣿⣤⣤⣄⠀⠀⠀
        ⠀⣰⣾⣿⣿⣿⣿⣿⣿⣿⡎⢿⣿⠁⠀⢸⠀⢸⠁⠀⠀⠹⣿⠋⠸⠀⠐⢋⠇⣠⠀⣀⡄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢘⣿⣿⣿⣿⣿⣿⣷⠀⠀
        ⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⢀⣁⣀⣀⣼⠀⢸⣤⣤⣤⣤⣤⣤⣤⠀⠀⠸⠊⠘⠊⠈⠉⠉⠉⠁⠀⠀⠉⡝⢷⣶⣶⣶⡶⠂⣤⣀⠀⡀⢀⣿⣿⣿⣿⣿⣿⣿⠁⠀
        ⠀⠘⠿⢿⠿⢿⣿⣿⣿⣿⣷⢻⡿⠻⣿⣿⠀⢸⣿⣿⡟⢻⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡐⠀⠈⢻⣿⡿⠁⠀⠹⣿⣿⣿⣿⣿⣿⡏⠻⠿⠛⠁⠀⠀
        ⠀⠀⠀⠀⠀⠘⢿⣿⣿⣿⣿⢸⣇⢠⣿⡿⠀⢸⣿⣿⣄⣠⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣎⠀⠀⠀⢠⣿⡇⠀⠀⢀⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⡿⠞⠿⠟⠛⡇⠀⣾⣿⣿⣿⣿⡏⠀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣠⣿⣿⣿⣄⣀⣰⣿⣿⣿⡄⢀⣾⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⡖⠉⠉⠉⠉⢀⣀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⣿⡿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣤⣤⣀⡀⠀⠀
        ⠠⣴⣾⣿⣷⡀⠀⣠⡾⠻⣿⣷⣄⡀⠀⡏⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⢀⣤⢶⡦⢀⣴⠶⣶⡄⠀⠀⠀⠀⠀⠀⠀⠀⢠⣤⡤⠀⠀⠈⠛⠉⠙⠿⠛⠿⢿⣿⣿⣿⠇
        ⠀⠈⢻⣿⡏⠀⣰⣿⠃⠀⠈⣿⠟⠁⢰⠃⠀⣀⢀⠀⠀⢀⠄⠀⢀⡀⢸⣿⣤⠀⣸⣿⠴⢛⣧⠀⠀⡀⠀⣀⠀⠀⠀⢈⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠁⠀
        ⠀⠀⢸⣿⡇⠀⣿⣿⠀⠀⠀⠀⠀⠀⢸⠀⢾⣿⠻⠗⠸⣿⣄⣼⠏⠐⢻⣿⠀⠐⣿⣿⠀⣿⡏⠀⣾⡟⢻⣿⠀⣾⡿⢻⣿⡇⣰⡶⢶⣄⢠⣶⣶⣶⡀⠀⣿⡟⠀⠀
        ⠀⠀⢸⣿⡇⠐⣿⡟⠀⠀⢀⣀⣀⡀⢸⠀⢸⣿⠀⠀⠀⢹⣿⠏⠀⠀⢸⣿⠀⠀⢻⣿⠀⣼⣿⠀⣿⡇⢸⣿⠀⣿⡇⢸⣿⡇⣿⡇⢸⣿⠀⣿⡇⠛⠁⠀⣿⡇⠀⠀
        ⠀⠀⢸⣿⡇⠀⣿⣧⠀⠈⢹⣿⣿⠁⠸⠀⠼⠿⠄⠀⣀⡼⠋⠀⢀⣀⣸⡿⠀⣀⣼⡏⠀⠛⠛⠠⠟⠓⠾⠿⠄⢿⡷⣾⣿⡇⢿⣇⣸⡿⢀⣿⡇⠀⠀⠀⣿⡇⠀⠀
        ⠀⠀⣸⣿⡇⠀⢿⣿⣇⠀⠀⣿⣿⡀⠀⠀⠀⠀⠙⠟⠋⠀⠀⠀⠀⠙⠛⠁⠀⠙⠛⠁⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠀⠉⠉⠙⠂⠀⠸⣿⡇⠀⠀
        ⠀⠀⣿⣿⡇⠀⠈⣿⣿⣦⣀⣿⡿⠷⠀⣦⣤⣴⣶⣶⣶⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣶⣶⣶⣶⣶⣶⣶⣤⣤⣤⣤⣤⣄⠀⠀⠀⠀⠀⣿⡇⠀⠀
        ⠀⠀⣿⣿⡇⠀⠀⠘⠿⠛⠉⠀⠀⠀⠀⣿⣿⣿⣟⢿⠏⠉⠉⠉⢻⣿⣿⣁⣿⡏⠙⢤⡀⠀⠀⠀⠈⢫⡉⢉⣍⣉⣿⣿⣿⣿⡟⠿⠿⢿⣿⣿⣶⣶⣶⣤⣿⣇⠀⠀
        ⠀⠀⣿⠿⣇⣀⡀⠀⠀⠤⠔⠒⠒⠒⠺⣿⣿⣿⣿⣷⡄⠀⠀⠀⠈⠿⠿⠿⢿⡇⣠⣿⣿⣦⡀⠀⠀⠀⠈⠉⠻⣿⣿⣿⣿⠏⠀⠀⠀⠀⠉⠉⠙⠛⠛⠛⠿⠿⠀⠀
        ⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣦⣀⣀⠀⠀⠀⡄⣨⡇⠘⣿⣿⡿⢙⣦⣀⣀⣀⣢⣠⣿⣿⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣮⣙⠿⣿⡇⠀⠈⢟⣤⣿⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠿⠋⠙⢿⣿⣿⣿⣷⣮⣅⣠⣴⣿⣿⣿⣿⠟⠉⠀⠛⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
      )" << '\n';
      cout << "\n\n";
      cout << "Gryffindor insignia successfuly built. Continuing formation process..." << endl;
      usleep(2500000);
      system("clear");
      cout << "Building Hufflepuff insignia..." << endl;
      usleep(1500000);
      system("clear");
      cout <<  R"(
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡠⣺⣷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣶⣒⣒⣮⣭⣾⡿⢿⣿⣯⣕⡒⢢⣴⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣤⣴⣦⣶⣶⣶⣿⣿⣿⡟⠉⠀⠠⢂⡈⠙⣿⣿⣿⣿⣿⣶⣶⣶⣦⣤⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣶⣿⣿⣿⣿⣿⣿⣿⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣩⣭⣥⣿⣿⣻⣿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡶⡦⢄⣀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⡿⣿⠿⣻⣿⣿⣿⣿⣿⣿⣿⣿⡟⠛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⢿⡿⠿⣿⣿⣿⣿⣿⣿⣷⣷⣿⡿⣃⣴⠏⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⢿⣿⣿⣍⠉⠀⠹⣿⣿⣷⡄⠀⠈⢿⣿⣿⠀⠁⠀⣀⠀⠈⠁⠀⠀⠀⠈⠿⠉⠛⠿⣿⣿⣾⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⡏⢻⣿⣿⣧⡀⠀⠈⢿⣿⣿⣆⠀⠀⠻⣿⠰⠂⠞⣿⠇⠰⠀⡤⡿⡄⠀⠀⠀⣠⠀⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⢀⣀⣠⣴⣿⠿⣹⣿⣿⡀⠀⠹⣿⣿⣿⣄⠀⢈⣿⡟⢿⣷⡀⠀⢹⠀⠀⠀⠙⠀⠀⠀⠀⠷⠀⠈⠁⠘⣿⠋⢿⣿⣃⡙⢨⡷⢄⡀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠈⢻⣾⣽⣵⣿⣿⣿⣿⣿⠲⢶⣼⣿⣿⣿⣦⣾⣿⣷⣜⣿⣷⣄⢸⢠⣦⡀⡀⢀⣠⡀⠀⠀⠀⢀⠀⠀⠈⠀⠘⢿⣿⣿⢷⣓⣤⣼⠏⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⡁⠻⣿⠀⡰⣹⠟⠛⠛⠿⣦⡉⠛⢿⣿⣿⣿⣾⠁⠟⠀⠁⠈⣿⠁⠘⠂⠰⣻⠒⠰⠆⠴⡷⠎⠛⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⡇⠀⠙⣧⡜⠂⣄⡆⠀⢠⣤⣯⣶⣄⠉⢻⣻⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠀⠀⠀⠋⠀⢸⣿⡿⣿⣿⠁⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣧⠀⠀⣯⢤⣴⣿⣿⡄⠘⠛⢛⣿⣿⣿⣦⡙⢿⣗⠤⣴⣿⠆⣤⢀⣴⣦⠀⣀⢠⣴⣄⠀⡀⠀⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⢹⡿⠙⣿⣿⣷⣄⣇⠙⠛⢿⣿⣇⠀⠀⠀⠈⠙⢿⣿⣷⣿⣿⠀⠈⠛⢦⣄⠀⠸⠇⠀⠁⠈⠺⠈⠀⠉⠀⣿⡿⡿⣿⣟⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣖⣿⣿⣿⣿⣯⡣⣄⠀⠻⣿⣄⠀⠀⣀⣀⣀⣉⣿⠟⢻⣶⣥⢴⣥⣏⣷⣄⢠⣴⡧⣄⣀⣀⡀⠶⠤⠿⠿⠧⣿⣇⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⢸⡿⢻⣿⣿⠙⣿⣿⣿⡿⠿⢷⣿⣿⡿⠛⠛⠛⣛⢿⣿⣧⣶⣾⣿⠿⠿⠛⠛⠛⠉⠉⠉⠉⠀⠀⠀⢀⣠⣤⣄⡀⢀⣘⡫⢖⢄⡀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⡀⠈⢻⡗⠅⡀⣼⡿⣥⢿⣶⣤⣶⣿⡿⠟⠛⠉⠁⠀⠀⠀⢀⠀⠀⠀⠀⠀⣀⢀⣠⠂⣾⣿⢸⡁⢰⣿⢹⠿⠂⠀⠑⣄⠀
          ⠀⠀⠀⠀⠀⠀⠀⢀⣿⣟⣫⣩⣩⣹⣽⣾⣿⣤⣼⣿⣼⣳⠯⠞⠟⣩⣶⡶⣠⡇⠀⠀⠀⠀⢀⣾⡟⢿⣆⠒⣿⠁⢹⣿⢠⣿⣿⠋⠁⣸⣿⠴⢶⡄⠀⠀⠘⡇
          ⠀⠀⠀⣀⣀⣀⣀⣟⣽⣯⡟⣽⢫⣿⣿⣿⡿⠟⠛⠉⠉⢠⣶⡶⢲⣿⢈⠘⣿⡇⢀⣴⢿⣦⠉⣿⡇⢸⣿⣤⣿⠀⢸⣿⠀⢻⣿⠀⠈⢹⣿⠀⠙⠃⠀⠀⡸⠁
          ⠀⠀⠀⢳⠀⠀⠀⣉⡼⠀⢉⣨⣽⠋⢹⡇⠀⠀⠀⠀⠀⣿⣿⡴⣸⣿⠛⠀⣿⡇⢸⣏⡼⠛⠀⣿⡷⢾⠟⠸⢿⡶⠿⠿⠋⢸⣿⡄⠀⢸⣿⠀⠀⠀⠀⢰⠇⠀
          ⠀⠀⠀⣼⡐⢶⣿⣿⡇⠘⢹⣿⣿⠀⢸⠁⢀⣀⢠⣾⡷⣿⣿⠀⢸⣿⠀⠀⣿⡇⢸⣟⢀⣤⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⣄⣼⣿⠁⣆⣸⡿⠀⠀⠀⠀⢸⠀⠀
          ⠀⠀⠀⢸⡅⢸⣿⣿⡇⠀⢸⣿⣏⠀⢸⠆⢹⣿⠀⣿⡇⣿⣿⠀⢸⣿⠀⢀⡿⠗⠈⠛⠚⠁⢠⣿⠧⠄⠀⠀⠀⠀⢀⣀⣈⣉⣀⣀⣌⣉⣀⡀⡀⠀⠀⢸⠀⠀
          ⠀⣠⣾⢿⠛⢻⣿⣿⡿⠶⣬⣿⣿⠀⢸⠀⢸⣿⣠⣿⡇⣿⣿⠀⣸⣿⠀⠉⠀⠀⠀⠀⠀⠀⠞⣁⣠⣤⣴⣶⣿⣿⣿⣿⣯⡉⢿⣿⣿⣿⡟⠉⢹⡒⢄⠸⡇⠀
          ⠰⣿⣇⣸⣿⠸⣿⣿⡇⠀⠀⣿⣿⠶⢸⠀⠘⠛⠉⠛⣀⣿⠏⠰⠿⠃⠀⠀⠀⣀⣠⣤⣴⡾⣿⡻⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⢾⣿⣿⣿⣇⠀⢸⠀⠈⢳⡇⠀
          ⠀⠙⠻⢿⠋⠀⣿⣿⡇⠀⠀⣿⣿⠀⢸⠀⠀⠀⠀⠀⠉⠁⣀⣀⣤⣴⠲⠎⠉⠀⠀⠈⠙⣥⣠⠐⣽⣿⣿⣿⣿⣿⣿⡸⠋⡥⣼⣿⣿⣿⣿⠀⣧⡟⣋⣹⣧⠀
          ⠀⠀⠀⢸⡆⠀⣿⣿⣧⠀⢸⣿⠿⠷⢸⠀⢀⣠⡤⠴⠚⠻⡏⣿⠻⣿⣷⣀⠀⠀⠀⠀⠀⡠⢻⣦⣜⣿⣿⣿⣿⣿⣿⣠⣾⣿⣿⣿⣿⣿⣿⠀⠙⠤⠶⠟⠃⠀
          ⠀⠀⠀⢸⠧⡞⠋⠀⠀⠰⠋⠀⠀⠀⣸⠟⠉⠉⠀⠀⠀⠀⠀⠙⣦⣬⣿⣿⣷⣄⡤⠂⠴⠆⠂⠙⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠛⠛⠒⠒⢲⣶⣶⣶⣾⣿⠋⣭⣿⣄⣤⣤⣿⣤⣀⣤⣞⣋⣉⣻⣽⣿⣿⣿⣷⣶⠦⣄⡼⢛⣯⣽⣿⣿⣿⣿⣿⣿⣅⢸⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠸⣿⣿⣿⣿⣿⡇⠀⠟⠀⠀⠀⢿⢞⣟⡿⣻⣿⣿⣿⣿⠏⡏⠻⣿⣿⢾⢯⣼⣿⣯⣿⣿⣿⡿⠛⢿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠈⠛⠚⠓⠛⠞⠋⠉⠁⠀⣇⠀⠙⢷⣿⣾⡵⡯⠞⢿⣿⣿⣆⠀⠀⢙⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣿⣿⣿⣿⣿⣦⣴⣷⠦⢠⡄⣠⣾⢤⢠⣤⣼⡷⡄⣿⣧⡀⠈⠻⣿⣿⣆⠀⠀⢹⣿⣿⣷⣶⣿⣿⡿⢿⣿⡿⠃⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⢿⣭⣽⣿⣿⣿⣿⣷⣶⣦⣤⣿⣀⣀⠀⠘⠟⠀⣿⣿⣿⣆⣀⣹⣿⣿⣿⣿⣿⣿⣿⣿⠿⣟⣿⣧⣾⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⢿⣿⣯⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⡏⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠛⠻⠿⢿⣿⣿⠿⣿⣿⣿⣿⣿⣿⡿⢟⣿⣿⣿⡿⠿⠛⠋⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠻⣿⣿⣿⣿⣿⣿⣿⡿⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
          ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
            )" << '\n';
      cout << "\n\n";
      cout << "Hufflepuff insignia successfuly built. Continuing formation process..." << endl;
      usleep(2500000);
      system("clear");
      cout << "Building Ravenclaw insignia..." << endl;
      usleep(1500000);
      system("clear");
      cout << R"(
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⢰⣄⠀⠀⠀⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢲⣤⡀⠀⠀⢀⠀⠀⠀⠈⡇⣿⡄⠀⢰⡇⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣿⣄⠀⢸⣷⡀⠀⠀⡇⣿⣷⠀⣾⢣⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⡄⠀⣿⣧⠀⣿⡇⣿⣿⣴⡏⣼⡿⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣄⣿⣿⢀⣿⠇⣿⣿⡟⢰⣿⣇⡜⣱⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⡿⢰⣿⡿⢠⣿⣿⠟⣼⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⡿⠁⣾⣿⢁⣿⣿⢏⣼⣿⣟⣠⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣶⣶⣤⣤⣤⣤⣤⣴⣶⣶⣶⣿⡇⣿⣿⣿⣿⡿⠁⣸⣿⡇⣾⡿⢁⣾⣿⣿⠟⡡⢠⣤⣤⣄⣀⣀⣠⣤⣤⣤⣶⣶⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢱⣿⣿⣿⡿⠁⢠⣿⣿⣧⠋⣠⣿⡿⢋⣵⠞⣡⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⣼⣿⣿⣿⢡⣾⣿⣿⣿⣯⣼⠿⣫⣶⣿⣯⣤⣶⠀⠿⠿⠿⠿⠿⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⣀⣤⣤⠤⠤⠤⢾⣿⣿⣿⣿⣿⠰⣶⡄⠰⢂⣤⡄⣿⣿⣿⡇⢸⣿⣿⣿⣿⢟⣥⣾⠿⢋⣥⣾⡿⠋⡘⣿⠀⠘⣣⡄⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⣾⠉⠀⢀⣀⣤⣤⣤⣤⣀⠀⠈⠉⢻⡈⠃⣴⣿⣿⡇⢻⣿⣿⡇⠸⢹⣿⣿⣵⣿⠟⣡⣾⣿⣿⡿⢛⣿⠃⠛⣐⣚⣛⣃⣿⣿⠿⠿⠿⠤⠤⢄⣀⠀⠀⠀⠀
        ⠀⢀⣠⣿⣶⣿⣿⣿⡿⠋⠹⣿⣿⣷⡄⠀⢸⠛⠒⠒⠶⠶⠶⠾⠿⠿⠿⠀⠿⠿⠿⠟⠁⠾⠿⠿⠟⠉⠐⠛⠋⠉⡉⠉⠉⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠈⠹⣆⠀⠀
        ⢠⡿⢋⣿⡀⢸⣿⣿⡇⠀⠀⢹⣿⣿⣿⡄⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡇⠀⠀⠀⠀⠀⠀⠀⠀⢴⣿⣷⡀⠀⢠⡏⠀⠀
        ⣿⠀⣿⣿⣿⡎⣿⣿⣿⠀⠀⠀⣿⣿⣿⡇⣿⠀⠀⢀⡀⠀⠀⠀⠀⠀⡀⠀⠀⣀⡀⠀⠀⠀⢀⡀⠀⠀⣀⣀⠀⣿⡇⠀⣠⣤⣄⠀⢀⡀⠀⠀⢻⣿⡇⢀⣾⣅⠀⠀
        ⢻⣷⣤⣿⠟⠀⣿⣿⣿⠀⢀⣼⣿⣿⠏⠀⣿⠀⣾⠟⢻⣷⡄⣿⡇⠀⣿⢠⣾⠙⣿⡆⢺⡗⢻⣷⢠⣾⠉⠛⠁⣿⡇⠘⠏⠁⣿⣿⣿⡇⢰⡇⠀⣿⡇⣼⠁⠈⢳⡄
        ⠀⠈⠉⣿⠀⠀⢻⣿⣿⠙⢿⣿⣦⡀⠀⠀⣿⠀⣠⡤⢸⣿⡇⣿⡇⠀⣿⣿⣿⠋⠙⠁⢸⡇⠘⣿⢸⣿⠀⠀⠀⣿⡇⢠⣾⠃⣿⣿⣿⡇⢸⣿⢠⡿⢡⡇⠀⠀⢠⡟
        ⠀⠀⠀⣿⠀⠀⢸⣿⣿⠀⠀⣿⣿⣿⡄⠀⣿⠀⢿⣧⣸⣿⠇⢻⣧⡴⠋⠻⣿⠀⡶⠀⣸⡇⢀⣿⠘⠿⣄⠜⠀⣿⡇⠘⢿⡦⣿⠟⠛⠿⠋⠘⠛⠁⢸⠇⠀⢠⡟⠀
        ⠀⠀⠀⣿⠀⠀⢸⣿⣿⠀⠀⣿⣿⣿⣿⠀⣿⡄⠈⠉⠉⠁⠀⠀⠁⠀⠀⠀⠈⠉⠀⠀⠀⠀⠘⠁⠀⠀⣀⣀⣀⣀⣡⣤⣤⡤⣤⣤⣤⣤⣤⣤⣄⡀⢸⠀⠀⡟⠀⠀
        ⠀⠀⠀⢿⠀⠀⢸⣿⡏⠀⠐⠛⠛⠿⠿⠇⣿⣟⠓⠒⠶⠶⠦⠤⠤⠤⣤⡤⠶⠶⠖⠒⢒⣶⠟⠋⢹⣿⣿⣿⠿⡿⢯⣿⣾⣿⢸⣿⣿⣿⣿⡏⠀⠙⣿⠀⢸⠃⠀⠀
        ⠀⠀⠀⢸⣄⣀⣴⣿⣷⠶⣶⣶⣶⣤⣤⣤⣿⡏⡇⢸⣿⠀⣿⡇⢸⣇⣿⡇⠀⠀⣀⠀⢀⣤⡖⠀⣾⣿⣿⣿⣿⣭⣭⠽⣳⣿⢸⣿⣿⣿⣿⣷⣦⡤⠟⠀⢸⠀⠀⠀
        ⠀⠀⠀⠈⠉⠁⠀⣿⡏⠀⣿⣿⣿⣿⣿⡇⣶⣶⡇⠸⣛⣤⠭⠴⠶⠶⢾⡇⠀⢠⡿⠀⣾⣿⠗⠀⣿⣿⣿⣿⣿⣿⠗⣲⣼⣯⢸⣿⣿⣿⣿⣧⣤⡴⠚⠛⣿⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⣿⣿⣿⣿⣿⡇⣿⡿⢣⣾⢡⣄⣠⣤⣶⣦⣄⠀⢀⣿⣇⣼⣿⣿⣷⣾⣿⣿⣿⣿⣿⠿⢛⣴⣿⣿⢸⣿⣿⣿⣿⡇⠈⣷⣶⠾⠋⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⣿⣿⣿⣿⣿⡇⣿⢱⠟⠉⣩⣿⣿⣿⣿⣿⣿⡆⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢣⣿⣿⣿⡿⣼⣿⣿⣿⣿⡇⠀⣇⡎⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⢿⠁⠀⢿⣿⣿⣿⣿⣇⢸⡸⣆⣴⢋⣭⣭⡝⢿⣿⣿⣷⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢫⣾⣿⣿⣿⡇⣿⣿⣿⣿⣿⡇⠀⠹⠁⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⢸⣷⣮⣥⣾⡿⢋⡄⠀⣛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡛⢿⣧⡙⢿⣿⣿⢟⣥⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⡘⣿⣿⠿⣋⠀⣿⡇⠀⣿⡌⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣌⡛⠶⣌⡑⠿⢿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⣿⣿⡇⢛⡅⢸⣿⠀⣿⡇⢠⣿⡇⢸⡟⢿⡟⢽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⢮⣝⡳⢦⣬⣙⡛⠻⠿⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⠸⡇⢸⣿⠀⣿⡇⢸⣿⡇⣸⢃⣾⠁⣼⣿⣿⣿⢫⡭⠛⠿⡿⣿⡮⣝⠺⣽⣳⢦⣭⣝⣛⠿⠶⠦⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣇⠁⢸⣿⠀⣛⡃⢸⠿⣡⣯⣾⡟⢠⣿⣿⣿⠟⠊⣴⡇⢸⣿⠈⣙⠺⣽⣷⣿⣟⡷⠭⡝⠛⠃⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣆⢸⡇⣿⡿⠛⠷⠾⢿⣿⡿⢣⣿⣿⠟⣵⣶⠀⣿⡇⢸⣿⡇⠋⣼⣷⣮⣥⣤⣤⡶⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣆⠱⡜⠶⣠⠀⡠⠤⣀⠁⢸⣯⣃⠀⣿⣿⠀⣿⡇⢸⣿⢃⣾⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣷⡙⢿⣧⣭⣼⣿⡦⢱⣀⢏⣿⠀⣿⣿⠀⣿⡇⠘⣱⣿⣿⣿⣿⣿⣿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣿⣿⣿⣿⣦⣙⠿⠟⣩⠀⣾⡆⢸⣿⠀⣿⣿⠀⠟⣡⣾⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣿⣿⣿⣿⣿⣷⣤⡙⠀⣿⡇⢸⣿⠀⣿⠟⣠⣾⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣷⣦⣅⡘⢛⣠⣴⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠻⠿⣿⡿⠿⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
        )" << '\n';
        cout << "\n\n";
        cout << "Ravenclaw insignia successfuly built. Forming Hogwarts emblem..." << endl;
        usleep(2500000);
        system("clear");
        cout << "Building Hogwarts emblem..." << endl;
        usleep(1500000);
        animateEnding();
        cout << "🎉 Congratulations! You have successfully built the Hogwarts emblem, allowing you to exit "
            "The Hogwarts School of Witchcraft and Wizadry. Thank you for playing and we hope you enjoyed "
            "your adventure!" << endl;
}

void animateEnding() {
  string hogwarts = R"(
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣶⣶⣤⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣾⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣴⣿⠿⠛⠉⠉⠉⢦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣿⠟⠁⠀⠀⠀⠀⠀⠈⠣⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⣠⣾⣿⣿⣿⣿⡿⠿⢿⣿⣿⣿⣿⣿⣿⣷⣶⣦⣤⣄⡀⠀⠀⠀⣠⣤⣤⣤⣤⣶⡿⠋⣀⣠⣤⣄⣀⠀⠀⠀⠀⠀⠈⠢⣄⣀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⣠⣶⣶⣾⣿⣿⣿⣿⣿⣿⠀⣶⣦⠙⣿⣿⣿⠿⠻⠿⣿⣿⣿⣿⣿⣦⣀⣼⡿⠿⠿⢛⣿⣵⣶⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⢀⣴⣿⣿⣿⣿⣶⣄⠀⠀⠀⠀
    ⠀⠀⢰⣿⡿⠿⠿⢿⣿⣿⣿⣿⣿⣧⠙⢻⠀⣿⡿⠁⠀⠀⠀⢈⣿⣿⣿⣿⣿⣿⠋⠀⠀⣠⣾⠿⢻⣿⠿⠋⠁⠀⠈⣿⣿⣷⠀⣴⣿⣿⠟⠉⠉⠉⢿⣿⡀⠀⠀⠀
    ⠀⠀⢸⣿⡅⠀⠀⠀⠙⢿⣿⣿⣿⣿⣧⣾⢀⣿⡗⠀⠀⠀⠀⠀⢀⣈⣿⣿⣿⣿⠀⠀⠠⣿⣿⠿⠛⠁⠀⠀⠀⠀⣠⣿⣿⡟⣼⣿⡿⠁⠀⠀⠀⢀⣾⣿⠀⠀⠀⠀
    ⠀⠀⠘⢿⣷⣄⠀⠀⠀⠀⢻⣿⣿⣿⣿⡏⣼⣿⡆⠀⠀⠀⠀⢠⣤⣼⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⢀⣠⣴⣶⣿⣿⣿⠟⢰⣿⣿⠁⠀⠀⢀⣴⣿⡿⠁⠀⠀⠀⠀
    ⠀⠀⠀⠈⠻⢿⣷⣦⣀⠀⠀⣿⣿⣿⡿⢰⣿⣿⡟⠢⢀⠀⠀⠀⠀⠙⢿⣿⣿⣿⠀⠀⠀⠀⠀⣴⣾⣿⣿⣿⠿⠟⠋⠀⠀⢸⣿⡟⠀⢀⣴⡿⠟⠁⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠉⠛⠻⠇⠀⢸⣿⣿⠃⣾⣿⡟⠀⠀⠀⠀⠀⠀⣰⣶⣾⣿⣿⣿⠀⠀⠀⢠⣾⣿⣿⠟⠉⠀⠀⠀⠀⣀⡀⢸⣿⡇⠀⠘⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⡇⠻⠋⠀⠀⠀⢀⣶⣤⣀⡀⠉⠉⠻⣿⣿⠀⠀⠠⣿⣿⣿⣷⣶⡄⠀⣠⣶⣿⣿⣿⣾⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⡄⠀⠀⠀⠠⣿⣿⡿⡻⠿⠿⠿⠛⢿⣿⣶⠛⠻⠿⠟⠛⢿⣿⡿⣾⣿⠟⣛⣙⢿⣧⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⣿⠿⠃⠀⢀⠄⠀⢸⣿⣿⣿⡄⠀⢠⣷⣾⣿⣿⣿⡆⠀⠀⣷⣼⣿⢻⣟⣵⣿⡿⠿⣷⣭⡜⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⣿⡇⠀⣠⣴⣏⠀⢰⣿⣿⣿⣿⡇⠀⢸⣿⣿⠿⣿⣿⡇⠀⢸⡿⣿⡿⠾⢸⣿⣿⣤⣼⣿⣿⠛⣿⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣷⠀⠉⢻⣿⣧⡈⠛⠻⣿⣿⡇⠀⠈⠉⠀⠀⠈⠛⠃⠀⠈⠟⣸⣧⣤⣤⣭⣟⣛⣛⣻⣧⣤⣼⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿⡿⠿⠿⠿⣿⣿⣿⣿⣿⣶⣶⣿⣫⡄⠀⠀⣶⣶⣤⣤⣶⣦⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⢠⣾⣿⠏⠀⠀⠀⣀⣤⠴⣦⡀⠀⠀⢸⣿⣿⣿⠀⠀⣿⣿⣿⣿⣿⣿⠀⠀⣿⡟⠉⠁⠈⣯⠉⠛⠿⣿⣿⣿⣿⠀⠙⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⣰⣿⣿⠋⠀⠀⢀⣼⣿⡇⣴⣀⣙⣦⡀⣸⣿⣿⣿⠀⠀⣿⣿⣿⣿⣿⡟⠀⠀⣿⣿⣿⣿⣿⣄⠀⠀⠀⢹⣿⣿⠋⠀⠀⠈⢿⣿⣄⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⢀⣾⣿⣿⠃⠀⠀⢀⣾⣿⣿⣷⣬⡭⠿⠟⠁⣿⣿⣿⠏⠀⠀⠻⢿⣿⣿⡻⠃⠀⡀⠹⠟⣿⣿⣿⠏⠀⠀⠀⠘⠟⠁⠀⠀⠀⠀⠈⢿⣿⣷⣄⠀⠀⠀⠀
    ⠀⠀⠀⣴⣿⣿⠟⣿⣿⣶⣤⣿⣿⣿⣿⣿⣿⣶⣤⣀⠘⣿⣿⣷⣶⣶⠿⠟⠛⢻⣿⣿⣿⣿⡿⢶⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⡄⠀⠀⠀⠀⠀⠘⣿⣿⣿⣷⣄⡀⠀
    ⠀⣠⣾⣿⠟⠁⠀⠈⠻⢿⠿⢻⣿⣿⣿⣿⣿⣿⣿⣿⣦⣌⡙⠋⠁⠀⠀⠀⠀⢸⣿⣿⣿⣿⠁⠀⠈⠉⠀⠀⠀⠀⠀⠀⠀⠀⢳⢀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣆
    ⢾⣿⣿⡋⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣄⠀⠀⠀⠀⢸⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣸⣿⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⡿⠋
    ⠀⠙⢿⣿⣦⣠⣄⠀⠀⣀⣼⣿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⢸⣿⣿⣿⣿⠀⠀⠀⠀⠰⡄⠀⠀⠀⠀⠀⢠⣿⣿⣞⡀⡀⡀⡄⢸⣿⣿⣿⠏⠀⠀
    ⠀⠀⠀⠉⠻⣿⣿⣿⠟⠛⠛⠉⠁⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⢸⣿⣿⣿⣿⡆⠀⠀⠀⠀⣷⠀⠀⠀⠀⢀⣾⣿⣿⣿⣷⣷⡇⢹⣸⣿⡟⠁⠀⠀⠀
    ⠀⠀⠀⠀⠀⠈⠻⣿⣄⠀⢀⣠⣄⠀⠀⠀⠀⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⢸⣿⣿⣿⣿⣷⠀⠀⡀⣷⣿⣧⠀⣀⡀⢸⣿⠟⣿⣿⣿⣿⣿⣼⣿⠋⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠘⣿⣶⣿⣿⣿⣷⣄⠀⠀⠀⠀⠈⣿⣿⣿⣿⣿⢿⣿⣆⠀⢸⣿⣿⣿⣿⣿⡄⡄⣿⣿⣿⡟⢰⣿⡇⠈⣱⣷⣿⠿⢿⣿⣿⡿⠁⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠸⠿⠋⠀⠙⢿⣿⣧⡀⣠⣤⣤⣿⣿⡟⣿⣿⡀⢿⣿⡆⢸⣿⣿⣿⣿⣿⣇⣷⣿⣿⡟⢠⣿⡟⣠⣧⣿⡟⠁⠀⠀⠉⠛⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⢿⣿⣿⣿⠋⢉⣁⣿⣿⠷⠀⠉⠁⢸⣿⣿⣿⣿⣿⣿⣿⡿⠋⣠⣶⣬⣷⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⢿⣿⣿⣿⣅⡀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣱⣾⣿⣿⡿⠟⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠛⠿⣿⣷⣤⡀⢸⣿⣿⣿⣿⣿⡿⠟⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣼⣿⣿⣿⠟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⡿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    )";
   string gryffindor = R"(
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣤⣤⣤⣶⣶⣶⣶⣶⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣶⣶⣶⣶⣦⣤⣤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⡟⠛⠋⠉⠉⣹⡍⠁⠀⠀⠀⢠⣿⡇⣶⣶⣶⣶⣾⣿⣿⣿⣭⣽⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣷⡄⠀⠀⢰⣿⣿⠄⠀⠤⣤⣭⣟⣡⣽⣻⣿⣿⣿⣿⡟⠈⣿⣿⣿⢹⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣄⢠⣿⢿⣋⣾⣿⣿⣿⣿⣿⣿⠛⠿⠊⠻⢿⣿⣿⣶⣿⣿⣿⣏⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⣿⣿⣿⠃⠀⠵⣿⣿⣿⣿⣿⣿⠿⠓⠀⠀⠀⠤⣤⠄⠉⠛⢻⣿⣿⣿⣿⡜⣿⣿⣿⣿⣦⣀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⣠⣦⣤⣤⣴⣾⣿⣿⣿⣿⣿⣿⠃⠀⠀⣾⣿⣿⣿⣿⣿⠠⠶⠆⠀⠀⠀⠀⠀⠀⠀⠀⠉⣿⣿⣿⣿⣿⣜⢿⣿⣿⣿⣿⣿⣿⣿⣆⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⢰⣿⣿⣿⣿⣿⣿⣿⣿⡿⠃⠈⠃⠀⠀⠼⣿⣿⣿⣿⣿⣿⣦⣄⣀⠀⠀⠀⠀⣴⣷⣶⣶⣴⣿⣿⣿⣿⣿⣿⠗⠀⠀⠈⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⢸⣿⣿⣿⣿⡿⢿⡟⠋⠀⠀⠀⣴⡀⢀⠾⣻⣿⣿⣿⣿⣿⣿⣿⣷⡆⠀⠀⠀⠻⠿⠿⠿⣿⣏⠀⣹⣿⣿⠇⠀⠀⠀⢰⣶⡎⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⢸⣿⣿⣿⣿⠀⣾⣿⣄⠀⠀⣼⣿⣿⢄⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⣿⣿⣷⣿⣿⣿⡄⠀⠀⠀⣿⣿⡇⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⢸⣿⣿⣿⣿⣾⣿⣿⣿⠆⠸⣿⣏⡵⠿⡻⣿⡿⣻⣿⣿⣿⣿⠋⢿⣿⣿⣷⣶⣶⣿⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⣰⣿⣿⡇⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠸⣿⣿⣿⣿⡏⣿⣿⡏⠀⠀⠙⣿⡿⠃⠀⠘⢱⣿⣿⣿⣿⠃⠀⠀⠻⢿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⠀⢠⣿⣿⣿⣿⡇⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⣿⣿⣿⠟⠉⠈⠙⠢⣄⠀⠀⠈⠁⠀⠀⠀⠘⢸⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀⠙⠻⠿⠿⢿⣿⡿⠃⠀⠀⠀⣸⣿⣿⢿⣿⡇⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⣿⣿⠃⠀⠀⢀⣤⠒⢄⠑⢄⣼⣦⠀⠀⠀⢠⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⡇⠀⠀⠀⢀⣿⣿⡁⢈⣿⡇⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⢹⣿⡄⠀⣾⢁⣾⣧⡀⠱⡀⠹⣿⣧⡀⢠⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠑⠤⢄⣀⣀⡀⢹⠀⠀⠀⣼⣿⣿⣷⣾⣿⢱⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⢸⣿⣿⣦⣽⣾⣿⣿⡗⠀⣷⠀⢹⡿⠁⠸⣿⣿⣿⣧⠀⠀⡠⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠒⠚⠛⠛⠛⠻⠿⠿⢸⣿⣿⣿⣿⣤⣤⣄⠀⠀⠀
    ⠀⣰⣾⣿⣿⣿⣿⣿⣿⣿⡎⢿⣿⠁⠀⢸⠀⢸⠁⠀⠀⠹⣿⠋⠸⠀⠐⢋⠇⣠⠀⣀⡄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢘⣿⣿⣿⣿⣿⣿⣷⠀⠀
    ⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⢀⣁⣀⣀⣼⠀⢸⣤⣤⣤⣤⣤⣤⣤⠀⠀⠸⠊⠘⠊⠈⠉⠉⠉⠁⠀⠀⠉⡝⢷⣶⣶⣶⡶⠂⣤⣀⠀⡀⢀⣿⣿⣿⣿⣿⣿⣿⠁⠀
    ⠀⠘⠿⢿⠿⢿⣿⣿⣿⣿⣷⢻⡿⠻⣿⣿⠀⢸⣿⣿⡟⢻⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡐⠀⠈⢻⣿⡿⠁⠀⠹⣿⣿⣿⣿⣿⣿⡏⠻⠿⠛⠁⠀⠀
    ⠀⠀⠀⠀⠀⠘⢿⣿⣿⣿⣿⢸⣇⢠⣿⡿⠀⢸⣿⣿⣄⣠⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣎⠀⠀⠀⢠⣿⡇⠀⠀⢀⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⡿⠞⠿⠟⠛⡇⠀⣾⣿⣿⣿⣿⡏⠀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣀⣠⣿⣿⣿⣄⣀⣰⣿⣿⣿⡄⢀⣾⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⡖⠉⠉⠉⠉⢀⣀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⣿⡿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣤⣤⣀⡀⠀⠀
    ⠠⣴⣾⣿⣷⡀⠀⣠⡾⠻⣿⣷⣄⡀⠀⡏⠀⠀⠉⠀⠀⠀⠀⠀⠀⠀⢀⣤⢶⡦⢀⣴⠶⣶⡄⠀⠀⠀⠀⠀⠀⠀⠀⢠⣤⡤⠀⠀⠈⠛⠉⠙⠿⠛⠿⢿⣿⣿⣿⠇
    ⠀⠈⢻⣿⡏⠀⣰⣿⠃⠀⠈⣿⠟⠁⢰⠃⠀⣀⢀⠀⠀⢀⠄⠀⢀⡀⢸⣿⣤⠀⣸⣿⠴⢛⣧⠀⠀⡀⠀⣀⠀⠀⠀⢈⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⠁⠀
    ⠀⠀⢸⣿⡇⠀⣿⣿⠀⠀⠀⠀⠀⠀⢸⠀⢾⣿⠻⠗⠸⣿⣄⣼⠏⠐⢻⣿⠀⠐⣿⣿⠀⣿⡏⠀⣾⡟⢻⣿⠀⣾⡿⢻⣿⡇⣰⡶⢶⣄⢠⣶⣶⣶⡀⠀⣿⡟⠀⠀
    ⠀⠀⢸⣿⡇⠐⣿⡟⠀⠀⢀⣀⣀⡀⢸⠀⢸⣿⠀⠀⠀⢹⣿⠏⠀⠀⢸⣿⠀⠀⢻⣿⠀⣼⣿⠀⣿⡇⢸⣿⠀⣿⡇⢸⣿⡇⣿⡇⢸⣿⠀⣿⡇⠛⠁⠀⣿⡇⠀⠀
    ⠀⠀⢸⣿⡇⠀⣿⣧⠀⠈⢹⣿⣿⠁⠸⠀⠼⠿⠄⠀⣀⡼⠋⠀⢀⣀⣸⡿⠀⣀⣼⡏⠀⠛⠛⠠⠟⠓⠾⠿⠄⢿⡷⣾⣿⡇⢿⣇⣸⡿⢀⣿⡇⠀⠀⠀⣿⡇⠀⠀
    ⠀⠀⣸⣿⡇⠀⢿⣿⣇⠀⠀⣿⣿⡀⠀⠀⠀⠀⠙⠟⠋⠀⠀⠀⠀⠙⠛⠁⠀⠙⠛⠁⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠀⠉⠉⠙⠂⠀⠸⣿⡇⠀⠀
    ⠀⠀⣿⣿⡇⠀⠈⣿⣿⣦⣀⣿⡿⠷⠀⣦⣤⣴⣶⣶⣶⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣶⣶⣶⣶⣶⣶⣶⣶⣤⣤⣤⣤⣤⣄⠀⠀⠀⠀⠀⣿⡇⠀⠀
    ⠀⠀⣿⣿⡇⠀⠀⠘⠿⠛⠉⠀⠀⠀⠀⣿⣿⣿⣟⢿⠏⠉⠉⠉⢻⣿⣿⣁⣿⡏⠙⢤⡀⠀⠀⠀⠈⢫⡉⢉⣍⣉⣿⣿⣿⣿⡟⠿⠿⢿⣿⣿⣶⣶⣶⣤⣿⣇⠀⠀
    ⠀⠀⣿⠿⣇⣀⡀⠀⠀⠤⠔⠒⠒⠒⠺⣿⣿⣿⣿⣷⡄⠀⠀⠀⠈⠿⠿⠿⢿⡇⣠⣿⣿⣦⡀⠀⠀⠀⠈⠉⠻⣿⣿⣿⣿⠏⠀⠀⠀⠀⠉⠉⠙⠛⠛⠛⠿⠿⠀⠀
    ⠀⠀⠀⠀⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣦⣀⣀⠀⠀⠀⡄⣨⡇⠘⣿⣿⡿⢙⣦⣀⣀⣀⣢⣠⣿⣿⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣮⣙⠿⣿⡇⠀⠈⢟⣤⣿⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⠿⠋⠙⢿⣿⣿⣿⣷⣮⣅⣠⣴⣿⣿⣿⣿⠟⠉⠀⠛⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⡟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
  )";
  string slytherin = R"(
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣠⣤⣄⣶⣶⣶⣶⣾⣿⣿⣿⣶⣶⣶⣦⣄⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⢰⣦⣄⣀⠀⠀⠀⠀⣀⣀⡀⠀⢀⣀⣤⣤⣴⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣤⣄⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⡿⠿⣟⠻⠏⢩⢹⣿⣿⣿⣿⡿⠿⢿⣿⡿⠿⠿⠿⠿⠿⠿⠿⠿⠿⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠏⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⢀⣘⠻⠯⢑⣊⣉⣀⠀⠀⢸⡘⠛⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣴⡆⠀⠀⠀⠀⠀⠀⠀⠀⠉⠉⠉⠙⠛⠛⠿⢿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⡏⠀⣠⡾⠛⠻⣿⡿⠟⠃⠸⡇⢀⣴⣷⠀⠀⠀⠀⠀⠀⢀⣴⣧⡀⢹⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠙⠻⢧⡀⠀⠀⠀⠀⠀
    ⠀⠀⡀⠄⠒⡇⠀⣿⣦⡀⠀⠉⠀⠀⠀⠀⡇⠀⣿⣿⠀⠀⣀⠀⠀⣀⢨⣿⡏⠀⢸⣿⣧⣴⣦⡀⠀⣴⣶⣶⣄⠀⣀⣴⣤⣤⡀⠀⣿⠇⠀⠀⠀⠀⠈⠧⠤⢄⡀⠀
    ⣠⠊⠀⠀⠀⠃⠀⠙⣿⣿⣶⣄⠀⠀⠀⠀⡇⠀⣿⣿⠘⣿⣿⠀⣿⡇⢸⣿⡇⠀⢸⣿⠃⢹⣿⡇⢸⣿⠁⣸⠿⠀⣿⣿⠛⠟⠁⣤⣴⡆⢀⣀⣠⡀⠀⠀⠀⠀⠉⡦
    ⠈⠢⣤⠀⠀⢸⠀⠀⠈⠻⣿⣿⣿⣦⡀⠀⠇⠀⣿⣿⠀⠸⣿⣦⣿⠁⠘⣿⣇⠄⣸⣿⡀⢸⣿⡇⢸⣿⡋⢀⡆⠀⣿⣿⠀⠀⢀⣿⡿⠀⢸⣿⡿⢻⣿⡆⠀⢀⠞⠁
    ⠀⠀⠀⠰⡀⣸⣤⣤⣤⣄⠈⠻⣿⣿⣧⠀⠀⠀⠾⠿⠂⠀⢹⡿⠃⠀⠈⠛⠁⠈⠉⠁⠀⠚⠛⠓⠈⠛⠷⠛⠁⠠⠿⠿⠄⠀⣼⣿⡇⠀⣸⣿⠁⢸⣿⠇⢠⠋⠀⠀
    ⠀⠀⠀⠀⣿⣿⠋⠁⠉⢿⣷⠀⠸⣿⣿⠀⢠⠀⠀⣴⣷⡾⠟⠁⣀⡤⠔⠒⠉⠉⠉⠉⠉⠓⠢⢄⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠁⠠⠿⠇⢀⣾⣿⠀⢳⠀⠀⠀
    ⠀⠀⠀⢸⣿⠁⠀⢦⣤⣾⡿⠀⢠⣿⣿⠂⢸⠀⣀⠠⣄⣒⢉⡿⠋⠀⠀⠀⢰⣄⣠⣀⣄⢀⠀⠀⠈⠣⣉⣲⢶⣤⣤⣀⣀⣀⠀⠀⠀⠀⠀⠀⠉⠙⠃⢀⡏⠀⠀⠀
    ⠀⠀⠀⢹⣿⣆⠀⠀⠀⠀⠀⢀⣾⣿⠟⠀⠸⠉⢰⣿⣿⢏⡞⠀⠀⠀⣠⣾⢟⣻⣯⣭⣟⡻⢿⣄⠀⠀⢶⣶⣍⠚⠋⠉⢽⢻⣷⣦⣄⡀⠀⠀⠀⠀⠀⢸⠀⠀⠀⠀
    ⠀⠀⠀⠀⠻⣿⣿⣶⣦⣴⡶⠟⠋⣁⣀⡤⠃⢀⣼⣿⡟⣼⡃⠀⠀⢰⡟⠀⠈⢻⡟⠁⠈⢻⣇⢿⡇⠀⣠⣤⠔⠒⠤⡂⢌⣿⣿⣿⣿⣿⣷⣦⣄⡀⠀⡜⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⡇⠀⠀⣀⠤⣒⣭⡍⢰⣾⣿⣿⣿⣿⣿⡇⡟⡁⠀⠀⢸⡇⣄⠰⣿⣿⠆⣴⣿⣿⣮⡳⡀⠹⡇⠐⠢⡔⣿⣿⣿⣿⡿⢿⣿⣿⣿⡇⠉⠙⠃⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠧⠒⠉⣶⣿⣿⣿⡇⠈⢿⣿⣿⣿⠿⢻⣇⢿⣷⠀⠀⠀⠙⢎⡳⣌⣡⣾⣿⣿⣿⣿⡇⠀⣥⣢⠅⣒⠛⣢⣽⡿⠋⠀⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⠀⣤⡙⣿⢱⢹⢸⣿⣌⢿⣷⣀⠀⠀⠀⠙⠢⢝⡻⢿⣿⣿⣿⣷⣠⣿⣿⣿⣶⣶⣿⣫⣶⣄⠀⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⢸⣿⣿⡌⢸⠀⣞⢿⣿⣷⣝⠿⣷⣦⣄⣀⡀⠀⠉⠐⠨⢝⡻⢿⣿⣿⣿⣿⣿⠟⠹⣿⣿⣿⠆⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⠀⢹⡟⡱⠟⣇⠘⢷⡒⢭⡻⣷⣮⣝⡻⣿⣿⣦⣀⠀⠀⠀⠉⠒⢭⡻⠿⢟⣅⠀⠀⣹⣿⡁⠀⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⡇⢠⡿⡸⠁⢸⡾⢦⡀⠳⡀⠹⡽⣿⣿⣿⣷⠍⠻⢿⣿⣶⣤⣀⠀⠀⠈⢳⣝⢿⡆⢴⣿⣿⣷⡄⢸⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⡇⠸⡇⡇⠀⢾⡀⣀⡷⠰⣧⠀⡇⣹⣿⣿⣿⣄⣠⣶⣭⡻⢿⣿⣷⠄⠀⠈⠻⢧⡀⠈⢻⡿⠋⠀⣼⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⡇⢀⣷⢻⣄⠀⠉⠉⠀⢀⡿⢠⡇⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣝⢿⣷⣄⠀⠀⠀⢻⣀⣾⣷⣄⠀⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⡇⠘⣿⣷⢉⣓⠶⠾⣴⣾⠃⢸⠇⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠀⠹⣿⣦⠀⠀⢈⣇⣿⣿⡿⠁⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⣼⣿⣿⣿⡇⠀⢈⣴⣿⣿⣿⣿⢹⣿⠀⢿⢠⣾⣿⣿⣿⣿⡿⣿⣿⣿⣿⣷⣄⣤⢻⣿⠀⠀⠸⣿⣹⣯⠀⠀⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⢰⣿⣿⣿⣿⡿⠿⢸⣿⠀⢸⣼⣿⣿⣿⠿⣫⣾⣮⠻⣿⣿⣿⣿⡿⣼⣿⠀⢠⣀⡇⣿⣿⣷⡄⣿⣿⣿⣿⡁⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⢸⣿⣿⣿⣿⡅⢀⢸⣿⠀⠸⣷⡻⠟⠁⠀⠹⣿⠋⠀⠈⠻⣿⠟⣱⣿⠏⢀⣿⣿⠁⣿⣿⣿⡇⣿⣿⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⡇⢸⣿⣿⣿⣿⣿⣿⡜⣿⣧⡀⠉⢹⢤⡀⠀⣴⣿⣦⠀⠀⣀⣴⡾⠟⠀⢀⣾⣿⣇⣰⣿⣿⣿⡇⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣷⡘⠻⢿⣿⣿⣿⣿⣿⣼⣿⣿⣶⡀⠀⠉⠛⠒⠒⠶⠟⠛⠋⠉⢀⣠⣶⣿⣿⣿⣿⣿⡿⢟⣫⣵⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣷⣦⣈⠙⠻⠿⠟⣱⣦⣍⡻⢿⣿⣿⣿⣆⣠⣶⣦⣤⣴⡾⠿⠛⣋⡻⣿⠿⣋⣥⣾⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠙⠻⢿⣿⣿⣿⣿⣿⣿⣿⣶⣤⣀⠙⠻⠿⠛⢷⣶⣬⡭⣭⣭⣭⡭⣥⣴⣶⠊⠻⢟⣫⣴⣾⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠋⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠛⠻⢿⣿⣿⣿⣿⣿⣿⣶⣤⣀⠈⠻⡏⠀⢘⣿⣟⠀⠈⡿⣋⣤⣶⣿⣿⣿⣿⣿⣿⣿⠿⠛⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⡚⠿⢟⣠⣴⣾⣿⣿⣿⣿⣿⣿⣿⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠿⣿⡿⠟⠋⠁⠀⠀⠀⠀⠀⠀		
    )";
    string ravenclaw = R"(
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⢰⣄⠀⠀⠀⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢲⣤⡀⠀⠀⢀⠀⠀⠀⠈⡇⣿⡄⠀⢰⡇⣦⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢻⣿⣄⠀⢸⣷⡀⠀⠀⡇⣿⣷⠀⣾⢣⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⡄⠀⣿⣧⠀⣿⡇⣿⣿⣴⡏⣼⡿⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣄⣿⣿⢀⣿⠇⣿⣿⡟⢰⣿⣇⡜⣱⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⡿⢰⣿⡿⢠⣿⣿⠟⣼⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⡿⠁⣾⣿⢁⣿⣿⢏⣼⣿⣟⣠⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣶⣶⣤⣤⣤⣤⣤⣴⣶⣶⣶⣿⡇⣿⣿⣿⣿⡿⠁⣸⣿⡇⣾⡿⢁⣾⣿⣿⠟⡡⢠⣤⣤⣄⣀⣀⣠⣤⣤⣤⣶⣶⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢱⣿⣿⣿⡿⠁⢠⣿⣿⣧⠋⣠⣿⡿⢋⣵⠞⣡⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠇⣼⣿⣿⣿⢡⣾⣿⣿⣿⣯⣼⠿⣫⣶⣿⣯⣤⣶⠀⠿⠿⠿⠿⠿⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⣀⣤⣤⠤⠤⠤⢾⣿⣿⣿⣿⣿⠰⣶⡄⠰⢂⣤⡄⣿⣿⣿⡇⢸⣿⣿⣿⣿⢟⣥⣾⠿⢋⣥⣾⡿⠋⡘⣿⠀⠘⣣⡄⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⣾⠉⠀⢀⣀⣤⣤⣤⣤⣀⠀⠈⠉⢻⡈⠃⣴⣿⣿⡇⢻⣿⣿⡇⠸⢹⣿⣿⣵⣿⠟⣡⣾⣿⣿⡿⢛⣿⠃⠛⣐⣚⣛⣃⣿⣿⠿⠿⠿⠤⠤⢄⣀⠀⠀⠀⠀
    ⠀⢀⣠⣿⣶⣿⣿⣿⡿⠋⠹⣿⣿⣷⡄⠀⢸⠛⠒⠒⠶⠶⠶⠾⠿⠿⠿⠀⠿⠿⠿⠟⠁⠾⠿⠿⠟⠉⠐⠛⠋⠉⡉⠉⠉⠀⠀⠀⠀⠀⠀⠀⢀⡀⠀⠈⠹⣆⠀⠀
    ⢠⡿⢋⣿⡀⢸⣿⣿⡇⠀⠀⢹⣿⣿⣿⡄⢸⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⡇⠀⠀⠀⠀⠀⠀⠀⠀⢴⣿⣷⡀⠀⢠⡏⠀⠀
    ⣿⠀⣿⣿⣿⡎⣿⣿⣿⠀⠀⠀⣿⣿⣿⡇⣿⠀⠀⢀⡀⠀⠀⠀⠀⠀⡀⠀⠀⣀⡀⠀⠀⠀⢀⡀⠀⠀⣀⣀⠀⣿⡇⠀⣠⣤⣄⠀⢀⡀⠀⠀⢻⣿⡇⢀⣾⣅⠀⠀
    ⢻⣷⣤⣿⠟⠀⣿⣿⣿⠀⢀⣼⣿⣿⠏⠀⣿⠀⣾⠟⢻⣷⡄⣿⡇⠀⣿⢠⣾⠙⣿⡆⢺⡗⢻⣷⢠⣾⠉⠛⠁⣿⡇⠘⠏⠁⣿⣿⣿⡇⢰⡇⠀⣿⡇⣼⠁⠈⢳⡄
    ⠀⠈⠉⣿⠀⠀⢻⣿⣿⠙⢿⣿⣦⡀⠀⠀⣿⠀⣠⡤⢸⣿⡇⣿⡇⠀⣿⣿⣿⠋⠙⠁⢸⡇⠘⣿⢸⣿⠀⠀⠀⣿⡇⢠⣾⠃⣿⣿⣿⡇⢸⣿⢠⡿⢡⡇⠀⠀⢠⡟
    ⠀⠀⠀⣿⠀⠀⢸⣿⣿⠀⠀⣿⣿⣿⡄⠀⣿⠀⢿⣧⣸⣿⠇⢻⣧⡴⠋⠻⣿⠀⡶⠀⣸⡇⢀⣿⠘⠿⣄⠜⠀⣿⡇⠘⢿⡦⣿⠟⠛⠿⠋⠘⠛⠁⢸⠇⠀⢠⡟⠀
    ⠀⠀⠀⣿⠀⠀⢸⣿⣿⠀⠀⣿⣿⣿⣿⠀⣿⡄⠈⠉⠉⠁⠀⠀⠁⠀⠀⠀⠈⠉⠀⠀⠀⠀⠘⠁⠀⠀⣀⣀⣀⣀⣡⣤⣤⡤⣤⣤⣤⣤⣤⣤⣄⡀⢸⠀⠀⡟⠀⠀
    ⠀⠀⠀⢿⠀⠀⢸⣿⡏⠀⠐⠛⠛⠿⠿⠇⣿⣟⠓⠒⠶⠶⠦⠤⠤⠤⣤⡤⠶⠶⠖⠒⢒⣶⠟⠋⢹⣿⣿⣿⠿⡿⢯⣿⣾⣿⢸⣿⣿⣿⣿⡏⠀⠙⣿⠀⢸⠃⠀⠀
    ⠀⠀⠀⢸⣄⣀⣴⣿⣷⠶⣶⣶⣶⣤⣤⣤⣿⡏⡇⢸⣿⠀⣿⡇⢸⣇⣿⡇⠀⠀⣀⠀⢀⣤⡖⠀⣾⣿⣿⣿⣿⣭⣭⠽⣳⣿⢸⣿⣿⣿⣿⣷⣦⡤⠟⠀⢸⠀⠀⠀
    ⠀⠀⠀⠈⠉⠁⠀⣿⡏⠀⣿⣿⣿⣿⣿⡇⣶⣶⡇⠸⣛⣤⠭⠴⠶⠶⢾⡇⠀⢠⡿⠀⣾⣿⠗⠀⣿⣿⣿⣿⣿⣿⠗⣲⣼⣯⢸⣿⣿⣿⣿⣧⣤⡴⠚⠛⣿⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⣿⣿⣿⣿⣿⡇⣿⡿⢣⣾⢡⣄⣠⣤⣶⣦⣄⠀⢀⣿⣇⣼⣿⣿⣷⣾⣿⣿⣿⣿⣿⠿⢛⣴⣿⣿⢸⣿⣿⣿⣿⡇⠈⣷⣶⠾⠋⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⣿⡇⠀⣿⣿⣿⣿⣿⡇⣿⢱⠟⠉⣩⣿⣿⣿⣿⣿⣿⡆⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢣⣿⣿⣿⡿⣼⣿⣿⣿⣿⡇⠀⣇⡎⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⢿⠁⠀⢿⣿⣿⣿⣿⣇⢸⡸⣆⣴⢋⣭⣭⡝⢿⣿⣿⣷⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢫⣾⣿⣿⣿⡇⣿⣿⣿⣿⣿⡇⠀⠹⠁⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⢸⣷⣮⣥⣾⡿⢋⡄⠀⣛⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡛⢿⣧⡙⢿⣿⣿⢟⣥⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⡘⣿⣿⠿⣋⠀⣿⡇⠀⣿⡌⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣶⣌⡛⠶⣌⡑⠿⢿⣿⣿⣿⣿⣿⠇⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⣿⣿⣿⣿⡇⢛⡅⢸⣿⠀⣿⡇⢠⣿⡇⢸⡟⢿⡟⢽⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⢮⣝⡳⢦⣬⣙⡛⠻⠿⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿⠸⡇⢸⣿⠀⣿⡇⢸⣿⡇⣸⢃⣾⠁⣼⣿⣿⣿⢫⡭⠛⠿⡿⣿⡮⣝⠺⣽⣳⢦⣭⣝⣛⠿⠶⠦⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣇⠁⢸⣿⠀⣛⡃⢸⠿⣡⣯⣾⡟⢠⣿⣿⣿⠟⠊⣴⡇⢸⣿⠈⣙⠺⣽⣷⣿⣟⡷⠭⡝⠛⠃⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣆⢸⡇⣿⡿⠛⠷⠾⢿⣿⡿⢣⣿⣿⠟⣵⣶⠀⣿⡇⢸⣿⡇⠋⣼⣷⣮⣥⣤⣤⡶⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠹⣿⣿⣿⣿⣿⣆⠱⡜⠶⣠⠀⡠⠤⣀⠁⢸⣯⣃⠀⣿⣿⠀⣿⡇⢸⣿⢃⣾⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣷⡙⢿⣧⣭⣼⣿⡦⢱⣀⢏⣿⠀⣿⣿⠀⣿⡇⠘⣱⣿⣿⣿⣿⣿⣿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠻⣿⣿⣿⣿⣿⣦⣙⠿⠟⣩⠀⣾⡆⢸⣿⠀⣿⣿⠀⠟⣡⣾⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣿⣿⣿⣿⣿⣷⣤⡙⠀⣿⡇⢸⣿⠀⣿⠟⣠⣾⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⢿⣿⣿⣿⣿⣿⣿⣷⣦⣅⡘⢛⣠⣴⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠋⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠻⠿⣿⡿⠿⠛⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    )";
    string hufflepuff = R"(
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⡠⣺⣷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢰⣶⣒⣒⣮⣭⣾⡿⢿⣿⣯⣕⡒⢢⣴⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣤⣴⣦⣶⣶⣶⣿⣿⣿⡟⠉⠀⠠⢂⡈⠙⣿⣿⣿⣿⣿⣶⣶⣶⣦⣤⣀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⣤⣶⣿⣿⣿⣿⣿⣿⣿⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣩⣭⣥⣿⣿⣻⣿⡿⣿⣿⣿⣿⣿⣿⣿⣿⣷⡶⡦⢄⣀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠙⣿⡿⣿⠿⣻⣿⣿⣿⣿⣿⣿⣿⣿⡟⠛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⢿⡿⠿⣿⣿⣿⣿⣿⣿⣷⣷⣿⡿⣃⣴⠏⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⢿⣿⣿⣍⠉⠀⠹⣿⣿⣷⡄⠀⠈⢿⣿⣿⠀⠁⠀⣀⠀⠈⠁⠀⠀⠀⠈⠿⠉⠛⠿⣿⣿⣾⣿⡿⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⣿⣿⣿⡏⢻⣿⣿⣧⡀⠀⠈⢿⣿⣿⣆⠀⠀⠻⣿⠰⠂⠞⣿⠇⠰⠀⡤⡿⡄⠀⠀⠀⣠⠀⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⢀⣀⣠⣴⣿⠿⣹⣿⣿⡀⠀⠹⣿⣿⣿⣄⠀⢈⣿⡟⢿⣷⡀⠀⢹⠀⠀⠀⠙⠀⠀⠀⠀⠷⠀⠈⠁⠘⣿⠋⢿⣿⣃⡙⢨⡷⢄⡀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠈⢻⣾⣽⣵⣿⣿⣿⣿⣿⠲⢶⣼⣿⣿⣿⣦⣾⣿⣷⣜⣿⣷⣄⢸⢠⣦⡀⡀⢀⣠⡀⠀⠀⠀⢀⠀⠀⠈⠀⠘⢿⣿⣿⢷⣓⣤⣼⠏⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⡁⠻⣿⠀⡰⣹⠟⠛⠛⠿⣦⡉⠛⢿⣿⣿⣿⣾⠁⠟⠀⠁⠈⣿⠁⠘⠂⠰⣻⠒⠰⠆⠴⡷⠎⠛⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⡇⠀⠙⣧⡜⠂⣄⡆⠀⢠⣤⣯⣶⣄⠉⢻⣻⣿⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠀⠀⠀⠀⠋⠀⢸⣿⡿⣿⣿⠁⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣧⠀⠀⣯⢤⣴⣿⣿⡄⠘⠛⢛⣿⣿⣿⣦⡙⢿⣗⠤⣴⣿⠆⣤⢀⣴⣦⠀⣀⢠⣴⣄⠀⡀⠀⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⢹⡿⠙⣿⣿⣷⣄⣇⠙⠛⢿⣿⣇⠀⠀⠀⠈⠙⢿⣿⣷⣿⣿⠀⠈⠛⢦⣄⠀⠸⠇⠀⠁⠈⠺⠈⠀⠉⠀⣿⡿⡿⣿⣟⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣖⣿⣿⣿⣿⣯⡣⣄⠀⠻⣿⣄⠀⠀⣀⣀⣀⣉⣿⠟⢻⣶⣥⢴⣥⣏⣷⣄⢠⣴⡧⣄⣀⣀⡀⠶⠤⠿⠿⠧⣿⣇⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⢸⡿⢻⣿⣿⠙⣿⣿⣿⡿⠿⢷⣿⣿⡿⠛⠛⠛⣛⢿⣿⣧⣶⣾⣿⠿⠿⠛⠛⠛⠉⠉⠉⠉⠀⠀⠀⢀⣠⣤⣄⡀⢀⣘⡫⢖⢄⡀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⡀⠈⢻⡗⠅⡀⣼⡿⣥⢿⣶⣤⣶⣿⡿⠟⠛⠉⠁⠀⠀⠀⢀⠀⠀⠀⠀⠀⣀⢀⣠⠂⣾⣿⢸⡁⢰⣿⢹⠿⠂⠀⠑⣄⠀
  ⠀⠀⠀⠀⠀⠀⠀⢀⣿⣟⣫⣩⣩⣹⣽⣾⣿⣤⣼⣿⣼⣳⠯⠞⠟⣩⣶⡶⣠⡇⠀⠀⠀⠀⢀⣾⡟⢿⣆⠒⣿⠁⢹⣿⢠⣿⣿⠋⠁⣸⣿⠴⢶⡄⠀⠀⠘⡇
  ⠀⠀⠀⣀⣀⣀⣀⣟⣽⣯⡟⣽⢫⣿⣿⣿⡿⠟⠛⠉⠉⢠⣶⡶⢲⣿⢈⠘⣿⡇⢀⣴⢿⣦⠉⣿⡇⢸⣿⣤⣿⠀⢸⣿⠀⢻⣿⠀⠈⢹⣿⠀⠙⠃⠀⠀⡸⠁
  ⠀⠀⠀⢳⠀⠀⠀⣉⡼⠀⢉⣨⣽⠋⢹⡇⠀⠀⠀⠀⠀⣿⣿⡴⣸⣿⠛⠀⣿⡇⢸⣏⡼⠛⠀⣿⡷⢾⠟⠸⢿⡶⠿⠿⠋⢸⣿⡄⠀⢸⣿⠀⠀⠀⠀⢰⠇⠀
  ⠀⠀⠀⣼⡐⢶⣿⣿⡇⠘⢹⣿⣿⠀⢸⠁⢀⣀⢠⣾⡷⣿⣿⠀⢸⣿⠀⠀⣿⡇⢸⣟⢀⣤⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⣄⣼⣿⠁⣆⣸⡿⠀⠀⠀⠀⢸⠀⠀
  ⠀⠀⠀⢸⡅⢸⣿⣿⡇⠀⢸⣿⣏⠀⢸⠆⢹⣿⠀⣿⡇⣿⣿⠀⢸⣿⠀⢀⡿⠗⠈⠛⠚⠁⢠⣿⠧⠄⠀⠀⠀⠀⢀⣀⣈⣉⣀⣀⣌⣉⣀⡀⡀⠀⠀⢸⠀⠀
  ⠀⣠⣾⢿⠛⢻⣿⣿⡿⠶⣬⣿⣿⠀⢸⠀⢸⣿⣠⣿⡇⣿⣿⠀⣸⣿⠀⠉⠀⠀⠀⠀⠀⠀⠞⣁⣠⣤⣴⣶⣿⣿⣿⣿⣯⡉⢿⣿⣿⣿⡟⠉⢹⡒⢄⠸⡇⠀
  ⠰⣿⣇⣸⣿⠸⣿⣿⡇⠀⠀⣿⣿⠶⢸⠀⠘⠛⠉⠛⣀⣿⠏⠰⠿⠃⠀⠀⠀⣀⣠⣤⣴⡾⣿⡻⣿⣿⣿⣿⣿⣿⡿⣿⣿⣿⢾⣿⣿⣿⣇⠀⢸⠀⠈⢳⡇⠀
  ⠀⠙⠻⢿⠋⠀⣿⣿⡇⠀⠀⣿⣿⠀⢸⠀⠀⠀⠀⠀⠉⠁⣀⣀⣤⣴⠲⠎⠉⠀⠀⠈⠙⣥⣠⠐⣽⣿⣿⣿⣿⣿⣿⡸⠋⡥⣼⣿⣿⣿⣿⠀⣧⡟⣋⣹⣧⠀
  ⠀⠀⠀⢸⡆⠀⣿⣿⣧⠀⢸⣿⠿⠷⢸⠀⢀⣠⡤⠴⠚⠻⡏⣿⠻⣿⣷⣀⠀⠀⠀⠀⠀⡠⢻⣦⣜⣿⣿⣿⣿⣿⣿⣠⣾⣿⣿⣿⣿⣿⣿⠀⠙⠤⠶⠟⠃⠀
  ⠀⠀⠀⢸⠧⡞⠋⠀⠀⠰⠋⠀⠀⠀⣸⠟⠉⠉⠀⠀⠀⠀⠀⠙⣦⣬⣿⣿⣷⣄⡤⠂⠴⠆⠂⠙⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠛⠛⠒⠒⢲⣶⣶⣶⣾⣿⠋⣭⣿⣄⣤⣤⣿⣤⣀⣤⣞⣋⣉⣻⣽⣿⣿⣿⣷⣶⠦⣄⡼⢛⣯⣽⣿⣿⣿⣿⣿⣿⣅⢸⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠸⣿⣿⣿⣿⣿⡇⠀⠟⠀⠀⠀⢿⢞⣟⡿⣻⣿⣿⣿⣿⠏⡏⠻⣿⣿⢾⢯⣼⣿⣯⣿⣿⣿⡿⠛⢿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⢿⣿⣿⣿⣿⣧⡀⠀⠀⠀⠀⠈⠛⠚⠓⠛⠞⠋⠉⠁⠀⣇⠀⠙⢷⣿⣾⡵⡯⠞⢿⣿⣿⣆⠀⠀⢙⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠈⢿⣿⣿⣿⣿⣿⣦⣴⣷⠦⢠⡄⣠⣾⢤⢠⣤⣼⡷⡄⣿⣧⡀⠈⠻⣿⣿⣆⠀⠀⢹⣿⣿⣷⣶⣿⣿⡿⢿⣿⡿⠃⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠻⢿⣭⣽⣿⣿⣿⣿⣷⣶⣦⣤⣿⣀⣀⠀⠘⠟⠀⣿⣿⣿⣆⣀⣹⣿⣿⣿⣿⣿⣿⣿⣿⠿⣟⣿⣧⣾⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠛⠻⢿⣿⣯⣿⣿⣿⣿⣿⣿⣿⣿⣶⣦⣄⡏⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠉⠛⠻⠿⢿⣿⣿⠿⣿⣿⣿⣿⣿⣿⡿⢟⣿⣿⣿⡿⠿⠛⠋⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⠻⣿⣿⣿⣿⣿⣿⣿⡿⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⣿⣿⣿⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
  ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠟⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    )";

    system("clear");
    for(int i = 10; i > 0; i--) {
        double delay = (i / 22.0);
        cout << slytherin << "\n\n";
        usleep(delay * 1000000);
        system("clear");
        cout << gryffindor << "\n\n";
        usleep(delay * 1000000);
        system("clear");
        cout << hufflepuff << "\n\n";
        usleep(delay * 1000000);
        system("clear");
        cout << ravenclaw << "\n\n";
        usleep(delay * 1000000);
        system("clear");
    }
    system("clear");
    cout << hogwarts << "\n\n\n";
}