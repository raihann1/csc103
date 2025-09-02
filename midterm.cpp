#include <iostream>
#include <vector>
using namespace std;
/*
Throughout the code, we are using user-defined functions 
for the purpose of making the code base clean. This was
implemented by Raihan from prior knowledge.
*/
string decryptText(string text, string key) {
    vector<char> textLetters;
    vector<char> keyLetters;
    int posInKey = 0;
    for (int i = 0; i < text.size(); i++) {
        // Input text will have disguised word breaks, thus, exclude spaces to make it readable.
        if (!isspace(text.at(i))){
            textLetters.push_back(text.at(i));
        }
    }
    for (int i = 0; i < textLetters.size(); i++) {
        // If we have not reached the end of the key, we can simply match the key with the first few letters.
        if (i < key.size()){
            keyLetters.push_back(key.at(posInKey));
            posInKey++;
        }
        // Now we have reached the end of the key, so we will use % to determine which keyLetter to match.
        else {
            keyLetters.push_back(key.at(posInKey % key.size()));
            posInKey++;
        }
    }
    /* The following method is inspired by a video explaining the concept & idea of the Vigenere cipher,
    YouTube video available @ https://www.youtube.com/watch?v=RCkGauRMs2A 
    Ideas used:
    - Convert characters to be from 0-25 instead of 97-122
    - To decrypt, take the encrypted char and subtract by the key at that index. 
      Then, mod 26 in case of overflow (mainly used for encryption)
    - Add 26 if the difference is negative (can happen during decryption). Then, add 'a' to convert from 0-25 to 97-122.
    */
    for (int i = 0; i < textLetters.size(); i++) {
        if (!isspace(textLetters.at(i))) {
            char newChar = ((textLetters.at(i) - 97) - (keyLetters.at(i) - 97)) % 26;
            /* If the difference is negative, moduluo will yield a negative value
               Therefore, add 26 since there are 26 letters in the alphabet.
               E.g. if newChar is -2, that means it is the second to last letter in the alphabet (y)
               With our system, y should be 24 (z = 25), therefore adding 26 will yield 24,
               which is later converted to an alphabetical letter (y).
            */
            if (newChar < 0) {
                newChar += 26;
            }
            newChar += 'a';
            textLetters.at(i) = newChar;
        }
    }
    string result;
    for (int i = 0; i < textLetters.size(); i++) {
        result += textLetters.at(i);
    }
    return result;
}

void analyzeText() {
    string encryptedText = "vome sxlzq dmxov qggz vqdx rdnpb uzefl dbbe yrgsk uakdp vcgap gvmaw uvncr eavyy qvdia apap gmsef gyaqp xuwu azgox fljlo tkxvf izwhd lroku lqgwg zgiip swgwt zwhdl rkvlx udlrs pampx ergha pgvmt zvuqp qvbvu apgz sywuv nvris dcnag fiequ looor gecys gnmal wyvys xukey ivmlr kkulg xxnlk vvucg egnsa";
    vector<int> freqs;
    vector<char> letters;
    for(int i = 97; i < 123; i++) {
        letters.push_back(i);
    }
    freqs.resize(letters.size());
    for(int i = 0; i < encryptedText.size(); i++) {
        if(isalpha(encryptedText.at(i))) {
            int letterPos = encryptedText.at(i) - 97;
            freqs.at(letterPos)++;
        }
    }
    for(int i = 0; i < freqs.size(); i++) {
        if(i != freqs.size() - 1) {
            cout << letters.at(i) << ": " << freqs.at(i) << ", "; 

        } else {
            cout << letters.at(i) << ": " << freqs.at(i) << endl;
        }
    }
}
/*
The code below makes use of static variables which was
inspired by the article: https://www.geeksforgeeks.org/static-keyword-cpp/
The reason we used a static variable is because we want to continously
access the previous number in our psuedo random sequence, so that our
encryption is reversible by Bob. When Bob uses the seed of our psuedo random
sequence--that Alice provides--to get the first number of the sequence, he needs 
to use that same number again to find the next number. Thus, it must be stored 
within the program so the genRandNum() function can produce the same exact sequence
Alice used to encrypt the message.
*/
static unsigned long int state;
void initalizeRand(unsigned long int seed) {
    state = seed;
}
unsigned long int genRandNum() {
    unsigned long int newNum = (((state - 1025) / 2) * 1337) + 91; // using random numbers / operations to generate a psuedo random number
    state = newNum;
    return newNum;
}

unsigned long int encryptText(string inputText) {
    unsigned long int seed = time(0);
    initalizeRand(seed);
    cout << endl;
    vector<int> keys;
    vector<char> text;
    for(int i = 0; i < inputText.size(); i++) {
        text.push_back(tolower(inputText.at(i)));
        if(isalpha(inputText.at(i))) {
             keys.push_back((genRandNum() % 25) + 1); // generate random # from 1 - 25 (at least 1 so each character has a shift)
       } else {
        // random number will never be negative, so use -1 to denote spaces
              keys.push_back(-1);
       }
    }
    char newChar;
    for(int i = 0; i < keys.size(); i++) {
        if(keys.at(i) != -1) {
            // if we are not at a space, shift
            if(keys.at(i) <= 12) {
                // special method for this type of encryption: if the random # is <= 12, shift BACKWARDS!
                newChar = (((text.at(i) - 97) - keys.at(i)) % 26);
                if(newChar < 0) {
                    newChar += 26; // possible that newChar could be negative due to subtraction
                }
                newChar += 97; // convert to an actual letter now!
            } else { 
                newChar = (((text.at(i) - 97) + keys.at(i)) % 26) + 97;
            }
            text.at(i) = newChar;
        }
    }
    for(int i = 0; i < text.size(); i++) {
        cout << text.at(i);
    }
    return seed;
}
void decryptCustomEncryption(string encryptedText, unsigned long int seed) {
    initalizeRand(seed);
    vector<int> keys;
    vector<char> text;
    for(int i = 0; i < encryptedText.size(); i++) {
        text.push_back(tolower(encryptedText.at(i)));
        if(isalpha(encryptedText.at(i))) {
             keys.push_back((genRandNum() % 25) + 1); // generate random # from 1 - 25 (at least 1 so each character has a shift)
        } else {
            keys.push_back(-1);
        }
    }
    char newChar;
    for(int i = 0; i < text.size(); i++) {
        if(keys.at(i) != -1) {
            if(keys.at(i) <= 12) {
                // our special shift condition, undo shift by going forward!
                newChar = (((text.at(i) - 97) + keys.at(i)) % 26);
                newChar += 97;
                text.at(i) = newChar;
            } else {
                // otherwise, undo the shift by subtracting
                newChar = (((text.at(i) - 97) - (keys.at(i))) % 26);
                if(newChar < 0) { // subtraction can make newChar negative!
                    newChar += 26;
                }
                newChar += 97;
                text.at(i) = newChar;
            }
        }
    }
    for(int i = 0; i < text.size(); i++) {
        cout << text.at(i);
    }
    cout << endl;

}
/*
The following code makes use of the cin.ignore() method due to the '\n'
character. When we run getline() after doing a regular cin >> varName,
getline() automatically returns "" and does not let the user input anything.
Hence, we use cin.ignore() before getline() so it discards the '\n' character.
Another solution was using getline() throughout the code instead of both
cin >> & getline, however, getline does not support integers which is one
of the values we read in. This is why we decided to use cin.ignore() over other
solutions.

This idea was adapted from a Stack Overflow post where a different person encountered a similar issue:
https://stackoverflow.com/a/18430444
*/
int main() {
    vector<string> possibleKeys = {"utter", "descriptive", "caring", "pies", "drawer", "station", "soothe", "year", "agreeable", "seemly", "button", "encourage", "reduce", "bed", "wave", "nosy", "zoo", "hateful", "flaky", "work", "ear", "uneven", "cumbersome", "languid", "box", "devilish", "yawn", "ablaze", "lake", "harbor", "legs", "glow", "glossy", "cruel", "warn", "hard", "unique", "card", "hug", "tangible", "hook", "label", "exotic", "account", "imagine", "grain", "tranquil", "book", "pickle", "whistle", "sack", "scissors", "trashy", "puzzled", "bottle", "smile", "neighborly", "eatable", "admit", "picayune", "type", "fast", "shy", "anger", "open", "add", "curly", "free", "aftermath", "cherry", "daily", "heal", "rose", "abhorrent", "short", "fine", "guarded", "vase", "fascinated", "fresh", "chickens", "mine", "stare"};
    string encryptedText = "vome sxlzq dmxov qggz vqdx rdnpb uzefl dbbe yrgsk uakdp vcgap gvmaw uvncr eavyy qvdia apap gmsef gyaqp xuwu azgox fljlo tkxvf izwhd lroku lqgwg zgiip swgwt zwhdl rkvlx udlrs pampx ergha pgvmt zvuqp qvbvu apgz sywuv nvris dcnag fiequ looor gecys gnmal wyvys xukey ivmlr kkulg xxnlk vvucg egnsa";
    /*
    The following loop was used to iterate through all
    possible keys. After manually looking through all combinations,
    the key "chickens" formed a coherent text.
    ---------------------------------------------
    for(int i = 0; i < possibleKeys.size(); i++) {
        string result = decryptText(encryptedText, possibleKeys.at(i));
        cout << "Trying key: " << possibleKeys.at(i) << " " << result << endl;
    }
    ---------------------------------------------
    */
   // BONUS: Suggest a key to the user based on frequency analysis!
    vector<int> frequenciesForKeys;
    frequenciesForKeys.resize(possibleKeys.size());    
    for(int i = 0; i < possibleKeys.size(); i++) {
        string result = decryptText(encryptedText, possibleKeys.at(i));
        for(int j = 0; j < result.size(); j++) {
            // use the top 3 frequent letters in english to find probable key
            if(result.at(j) == 'e' || result.at(j) == 't' || result.at(j) == 'a') {
                frequenciesForKeys.at(i)++;
            }
        }
    }
    int max = 0;
    int maxIndex = 0;
    for(int i = 0; i < frequenciesForKeys.size(); i++) {
        if(frequenciesForKeys.at(i) > max) {
            max = frequenciesForKeys.at(i);
            maxIndex = i;
        }
    }
    cout << "👋 Hello Alice and/or Bob!" << endl << "❓ What task would you like to perform today?" << endl;
    cout << "Part 1 - Analysis/Decryption 📊 🔓 (1)" << endl;
    cout << "Part 2 - Encryption 🔐 (2)" << endl;
    cout << "Please enter the task you wish to perform (integer input, 1 or 2 accepted only): ";
    int input;
    cin >> input;
    // while(input != 1 && input != 2) {
    //  cout << "❌ Invalid input! Please pick between Part 1 and Part 2 and enter the appropriate integer (1 or 2): ";
    //  cin >> input;
    // }
    /*
    The method system("clear") was adapted from the following article: 
    https://www.geeksforgeeks.org/how-to-clear-console-in-cpp/

    The purpose of incorporating this method is to clear
    the terminal and make it clean. Due to the many outputs
    of this program, it can get cluttered and hard to read!
    */
    system("clear");
    if(input == 1) {
     // Analysis & Decryption!
     cout << "Task 1: Frequency Analysis" << endl;
     cout << "------------------------------" << endl;
     cout << "Here is the list of how many times each letter appears in our encrypted text:" << endl;
     analyzeText();
     cout << "[BONUS]: Based on frequency analysis, our program believes the probable key for this encrypted text is: " << possibleKeys.at(maxIndex) << endl;
     cout << "------------------------------";
     cout << endl;
     cout << "Task 2: Decryption" << endl;
     cout << "------------------------------" << endl;
     cout << "The following text is encrypted with the Vigenère cipher: " << encryptedText << endl << endl;
     cout << "After going through all " << possibleKeys.size() << " possible keys (via brute force), our program decrypted the text! Here are our results:" << endl << endl;
     cout << "🔑 chickens: (raw, unformatted): " << decryptText(encryptedText, "chickens") << endl << endl;
     string formattedText = "the city however does not tell its past but contains it like the lines of a hand written in the corners of the streets the gratings of the windows the banisters of the steps the antennae of the lightning rods the poles of the flags every segment marked in turn with scratches indentations scrolls";
     cout << "🔑 chickens: (formatted with proper word breaks): " << formattedText << endl;
    } else if(input == 2){
     // Encryption!
     string input;
     cout << "👋 Hello! Please enter the text you wish to encrypt (text will be converted to lowercase): ";
     cin.ignore();
     getline(cin, input);
     cout << "Given the string \"" << input << "\", our system encrypted your message to be: ";
     unsigned long key = encryptText(input);
     cout << endl << endl << "The seed to decrypt the text is: " << key << endl << endl;
     cout << "Would you like to run our decryption tool? (y/n): ";
     char decryptionInput;
     cin >> decryptionInput;
     while(decryptionInput != 'y' && decryptionInput != 'n') {
        cout << "Invalid input! Please reply with y or n: ";
        cin >> decryptionInput;
     }
     if(decryptionInput == 'n') {
        cout << "Thanks for using our encryption system! Until next time. 👋" << endl;
     } else {
        string encryptedText;
        cout << "Enter encrypted text: ";
        cin.ignore();
        getline(cin, encryptedText);
        unsigned long seed;
        cout << "Enter seed: ";
        cin >> seed;
        cout << endl;
        cout << "🤖 After running our decryption system, we decrypted your text to be: " << endl;
        decryptCustomEncryption(encryptedText, seed);
        cout << endl << "Thanks for using our decryption system, we hope this helped! Until next time. 👋" << endl;
     }
    }
    return 0;
}