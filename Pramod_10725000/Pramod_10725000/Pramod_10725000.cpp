#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

struct Word {
    string name;
    string type;
    string definition;
};

void displayMenu();
void loadDictionary(vector<Word>& Dictionary);
void searchWord(vector<Word>& Dictionary);
void firstAndLast(vector<Word>& Dictionary);
string toLowerCase(string str);
string trim(string str);
string getFullTypeName(string abbreviation);
int countDefinitions(string definition);
void displayDefinitions(string definition);

/*
 * Function: main
 * Purpose: Main program (it shows menu and handles user input)
 * Input: None
 * Output: Returns 0
 * Author: P L MALALAGAMA
 * Date: 2026/jan
 */
int main() {
    vector<Word> Dictionary;
    int choice;

    do {
        displayMenu();
        cout << "\nEnter your choice: ";
        cin >> choice;

        // Check if input failed (non-numeric input)
        if (cin.fail()) {
            cin.clear(); // Clear error flag
            cin.ignore(10000, '\n'); // Discard invalid input
            cout << "Invalid choice. Please try again.\n";
            cout << "\n";
            continue; // Go back to start of loop
        }

        cin.ignore(); // Clear newline character

        switch (choice) {
        case 1:
            loadDictionary(Dictionary);
            break;
        case 2:
            searchWord(Dictionary);
            break;
        case 3:
            firstAndLast(Dictionary);
            break;
        case 4:
            cout << "Exiting program. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }

        cout << "\n";

    } while (choice != 4);

    system("pause");
    return 0;
}

/*
 * Function: displayMenu
 * Purpose: Shows menu options
 * Input: None
 * Output: None
 * Author: P L MALALAGAMA
 * Date: 2026/jan
 */
void displayMenu() {
    cout << "\n-------------------------------------\n";
    cout << "          DICTIONARY MENU            \n";
    cout << "-------------------------------------\n";
    cout << "1. Load new Dictionary\n";
    cout << "2. Search Word\n";
    cout << "3. First and Last\n";
    cout << "4. Exit\n";
    cout << "-------------------------------------\n";
}

/*
 * Function: trim
 * Purpose: This is exclude leading and trailing whitespace
 * Input: str - string to trim
 * Output: Trimmed string
 * Author: P L MALALAGAMA
 * Date: 2026/jan
 */
string trim(string str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    size_t end = str.find_last_not_of(" \t\r\n");

    if (start == string::npos || end == string::npos) {
        return "";
    }

    return str.substr(start, end - start + 1);
}

/*
 * Function: loadDictionary
 * Purpose: Reads dictionary file and loads words into vector
 * Input: Dictionary vector (passed by reference)
 * Output: None
 * Author: P L MALALAGAMA
 * Date: 2026/jan
 */
void loadDictionary(vector<Word>& Dictionary) {
    ifstream inputFile("dictionary_2026S0.txt");

    if (!inputFile) {
        cout << "Error: Unable to open dictionary file.\n";
        return;
    }

    Dictionary.clear();
    string line;

    cout << "Loading dictionary, please wait...\n";

    while (getline(inputFile, line)) {
        // Remove carriage return if exists
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Skip empty lines
        if (line.empty() || line.back() != ';') {
            continue;
        }

        Word newWord;

        // Line 1: name of the word (remove semicolon and trim)
        newWord.name = trim(line.substr(0, line.length() - 1));

        // Line 2: type of word
        getline(inputFile, line);
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        newWord.type = trim(line.substr(0, line.length() - 1));

        // Line 3: definition
        getline(inputFile, line);
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        newWord.definition = line;

        // Line 4: Empty line
        getline(inputFile, line);

        Dictionary.push_back(newWord);
    }

    inputFile.close();

    cout << "Dictionary loaded successfully! Total words: " << Dictionary.size() << "\n";
}

/*
 * Function: searchWord
 * Purpose: browsing words from dictonary without casesensitivity
 * Input: Dictionary vector
 * Output: None
 * Author: P L MALALAGAMA
 * Date: 2026/jan
 */
void searchWord(vector<Word>& Dictionary) {
    if (Dictionary.empty()) {
        cout << "Error 202601- no dictionary loaded -\n";
        return;
    }

    string searchTerm;
    cout << "Enter word to search: ";
    getline(cin, searchTerm);

    // Trim and convert search term to lowercase
    searchTerm = trim(searchTerm);
    string searchLower = toLowerCase(searchTerm);

    // First pass: count matches and total definitions
    int matches = 0;
    int totalDefs = 0;

    for (size_t i = 0; i < Dictionary.size(); i++) {
        string dictWordLower = toLowerCase(Dictionary[i].name);

        if (dictWordLower == searchLower) {
            matches++;
            // Count definitions in this entry
            string def = Dictionary[i].definition;
            if (!def.empty() && def.back() == ';') {
                def.pop_back();
            }
            totalDefs += countDefinitions(def);
        }
    }

    // If no matches found
    if (matches == 0) {
        cout << "word not found.\n";
        return;
    }

    // Display header
    cout << "\nWord Found - with " << totalDefs << " definition";
    if (totalDefs > 1) {
        cout << "s";
    }
    cout << "\n\n";

    // Second pass: display all matching entries
    for (size_t i = 0; i < Dictionary.size(); i++) {
        string dictWordLower = toLowerCase(Dictionary[i].name);

        if (dictWordLower == searchLower) {
            cout << Dictionary[i].name << " " << getFullTypeName(Dictionary[i].type) << "\n";

            // Remove trailing semicolon from definition
            string def = Dictionary[i].definition;
            if (!def.empty() && def.back() == ';') {
                def.pop_back();
            }

            displayDefinitions(def);
            cout << "\n";
        }
    }
}

/*
 * Function: firstAndLast
 * Purpose: Shows first and last word for each letter
 * Input: Dictionary vector
 * Output: None
 * Author: P L MALALAGAMA
 * Date: 2026/jan
 */
void firstAndLast(vector<Word>& Dictionary) {
    if (Dictionary.empty()) {
        cout << "Error 202601- no dictionary loaded -\n";
        return;
    }

    cout << "\n========== FIRST AND LAST WORDS BY LETTER ==========\n\n";
    cout << "Letter\t\tFirst Word\t\tLast Word\n";
    cout << "------\t\t----------\t\t---------\n";

    for (char letter = 'a'; letter <= 'z'; letter++) {
        string firstWord = "";
        string lastWord = "";

        for (size_t i = 0; i < Dictionary.size(); i++) {
            if (!Dictionary[i].name.empty() &&
                tolower(Dictionary[i].name[0]) == letter) {

                if (firstWord.empty()) {
                    firstWord = Dictionary[i].name;
                }
                lastWord = Dictionary[i].name;
            }
        }

        if (!firstWord.empty()) {
            cout << letter << "\t\t" << firstWord;

            if (firstWord.length() < 8) {
                cout << "\t\t\t" << lastWord << "\n";
            }
            else if (firstWord.length() < 16) {
                cout << "\t\t" << lastWord << "\n";
            }
            else {
                cout << "\t" << lastWord << "\n";
            }
        }
    }

    cout << "\n====================================================\n";
}

/*
 * Function: toLowerCase
 * Purpose: Converts string to lowercase
 * Input: str - string to convert
 * Output: Lowercase string
 * Author: P L MALALAGAMA
 * Date: 2026/jan
 */
string toLowerCase(string str) {
    string result = str;
    for (size_t i = 0; i < result.length(); i++) {
        result[i] = tolower(result[i]);
    }
    return result;
}

/*
 * Function: getFullTypeName
 * Purpose: Converts type abbreviation to full name
 * Input: abbreviation - type code
 * Output: Full type name in brackets
 * Author: P L MALALAGAMA
 * Date: 2026/jan
 */
string getFullTypeName(string abbreviation) {
    if (abbreviation == "v") return "(verb)";
    if (abbreviation == "n") return "(noun)";
    if (abbreviation == "adv") return "(adverb)";
    if (abbreviation == "adj") return "(adjective)";
    if (abbreviation == "prep") return "(preposition)";
    if (abbreviation == "pn") return "(proper noun)";
    if (abbreviation == "n_and_v") return "(noun and verb)";
    if (abbreviation == "misc") return "(miscellaneous)";
    return "(" + abbreviation + ")";
}

/*
 * Function: countDefinitions
 * Purpose: Counts definitions separated by ";  "
 * Input: definition string
 * Output: Number of definitions
 * Author: P L MALALAGAMA
 * Date: 2026/jan
 */
int countDefinitions(string definition) {
    int count = 1;

    // Look through the string for ";  " (semicolon followed by two spaces)
    for (size_t i = 0; i < definition.length() - 2; i++) {
        if (definition[i] == ';' &&
            definition[i + 1] == ' ' &&
            definition[i + 2] == ' ') {
            count++;
        }
    }

    return count;
}

/*
 * Function: displayDefinitions
 * Purpose: This Prints definitions with line breaks
 * Input: definition string
 * Output: None
 * Author: P L MALALAGAMA
 * Date: 2026/jan
 */
void displayDefinitions(string definition) {
    int defNumber = 1;
    size_t pos = 0;

    while (pos < definition.length()) {
        size_t nextPos = definition.find(";  ", pos);

        if (nextPos == string::npos) {
            // Last definition
            cout << "  " << defNumber << ". " << definition.substr(pos) << "\n";
            break;
        }
        else {
            // Print current definition
            cout << "  " << defNumber << ". " << definition.substr(pos, nextPos - pos) << "\n\n";
            pos = nextPos + 3;
            defNumber++;
        }
    }
}