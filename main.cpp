// Name: Neel Patel
// Class: Data Structures CS5393-002
// Assignment: Lab # 5

// DESIGN DOCUMENTATION

/*

This code implements a Trie (Prefix Tree) data structure for efficient insertion and retrieval of words from a dictionary dataset.

Design:

- TrieNode Class:

  - Contains an `unordered_map<char, TrieNode*>` called `children` to store child nodes corresponding to each character.
  
  - A boolean `isEndOfWord` to indicate if the node marks the end of a complete word.

- Trie Class:

  - Provides operations such as `insert(word)`, `search(word)`, `startsWith(prefix)`, and `findWordsWithPrefix(prefix)`.
  
  - Uses STL features like `unordered_map` for fast character mapping.
  
  - Includes proper message handling for errors, successes, and failures.
  
  - Exception handling and memory management are implemented, particularly when searching for words that do not exist in the Trie.

- Main Function:

  - Demonstrates inserting words into the Trie.
  
  - Performs searches for specific words, prefixes, and handles special cases like hyphenated words, words with numbers, mixed case words, etc.
  
  - Handles exceptions when searching for non-existent words.

Challenges and Solutions:

- Case Sensitivity:

  - To handle mixed case words, the Trie stores words in a case-sensitive manner.
  
- Special Characters:

  - The Trie is designed to handle words with hyphens, numbers, and other special characters by treating them as 
  valid characters in the `children` map.

*/

// Collaborators: Manny Garcia

// References:
// https://www.geeksforgeeks.org/trie-insert-and-search/
// https://cplusplus.com/reference/unordered_map/unordered_map/
// https://cplusplus.com/reference/string/string/
// https://www.cplusplus.com/reference/vector/vector/

// Code Implementation

// Include necessary header files
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <exception>
#include <fstream>   // For file I/O
#include <algorithm> // For std::transform

using namespace std;

// TrieNode class implementation remains the same
class TrieNode {
public:
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

    // Constructor
    TrieNode() : isEndOfWord(false) {}

    // Destructor to free memory
    ~TrieNode() {
        for (auto& child : children) {
            delete child.second;
        }
    }
};

// Trie class implementation
class Trie {
private:
    TrieNode* root;

    // Helper function for findWordsWithPrefix
    void findAllWords(TrieNode* node, string currentPrefix, vector<string>& words) {
        if (node->isEndOfWord) {
            words.push_back(currentPrefix);
        }
        for (auto& child : node->children) {
            findAllWords(child.second, currentPrefix + child.first, words);
        }
    }

public:
    // Constructor
    Trie() {
        root = new TrieNode();
    }

    // Destructor
    ~Trie() {
        delete root;
    }

    // Inserts a word into the trie
    void insert(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (!current->children.count(ch)) {
                current->children[ch] = new TrieNode();
            }
            current = current->children[ch];
        }
        current->isEndOfWord = true;
        cout << "Inserted word: '" << word << "' into the Trie." << endl;
    }

    // Searches for a word in the trie
    bool search(const string& word) {
        TrieNode* current = root;
        for (char ch : word) {
            if (!current->children.count(ch)) {
                cout << "Word '" << word << "' not found in the Trie." << endl;
                return false;
            }
            current = current->children[ch];
        }
        if (current->isEndOfWord) {
            cout << "Word '" << word << "' found in the Trie." << endl;
            return true;
        } else {
            cout << "Word '" << word << "' not found in the Trie." << endl;
            return false;
        }
    }

    // Checks if any word in the trie starts with the given prefix
    bool startsWith(const string& prefix) {
        TrieNode* current = root;
        for (char ch : prefix) {
            if (!current->children.count(ch)) {
                cout << "No words starting with prefix '" << prefix << "' found in the Trie." << endl;
                return false;
            }
            current = current->children[ch];
        }
        cout << "There are words starting with prefix '" << prefix << "' in the Trie." << endl;
        return true;
    }

    // Returns all words that start with the given prefix
    vector<string> findWordsWithPrefix(const string& prefix) {
        vector<string> words;
        TrieNode* current = root;
        for (char ch : prefix) {
            if (!current->children.count(ch)) {
                cout << "No words starting with prefix '" << prefix << "' found in the Trie." << endl;
                return words;
            }
            current = current->children[ch];
        }
        findAllWords(current, prefix, words);
        if (words.empty()) {
            cout << "No words starting with prefix '" << prefix << "' found in the Trie." << endl;
        } else {
            cout << "Words starting with prefix '" << prefix << "':" << endl;
            for (const string& word : words) {
                cout << word << endl;
            }
        }
        return words;
    }
};

// Function to read words from the dataset file
vector<string> readWordsFromFile(const string& filename) {
    vector<string> words;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return words;
    }

    string line;
    while (getline(file, line)) {
        // Trim whitespace from both ends of the line
        size_t start = line.find_first_not_of(" \t\r\n");
        size_t end = line.find_last_not_of(" \t\r\n");

        if (start == string::npos || end == string::npos) {
            continue; // Skip empty lines
        }

        string trimmedLine = line.substr(start, end - start + 1);

        // Skip lines that are headings (start with '#' or '##')
        if (trimmedLine[0] == '#') {
            continue;
        }

        // Now, the line is a word, so add it to the list
        words.push_back(trimmedLine);
    }

    file.close();
    return words;
}

int main() {
    // Read words from the dataset file
    string filename = "Lab5_dictionary-dataset.txt";
    vector<string> dataSet = readWordsFromFile(filename);

    if (dataSet.empty()) {
        cout << "No words were read from the file." << endl;
        return 1;
    }

    // Instantiate Trie
    Trie trie;

    // i. Insert words into the trie (all dataset)
    cout << "\nInserting words into the Trie:" << endl;
    for (const string& word : dataSet) {
        trie.insert(word);
    }

    // ii. Search for a word in the trie
    vector<string> searchWords = {
        "prefix", "data", "hello", "do", "workplace", "there",
        "pneumonia", "word2vec", "iPhone", "a-frame", "abc", "xyz"
    };

    cout << "\nSearching for words in the Trie:" << endl;
    for (const string& word : searchWords) {
        trie.search(word);
    }

    // iii. Return words that start with a given prefix
    vector<string> prefixes = {"pre", "pro", "work", "i", "e"};
    cout << "\nFinding words with given prefixes:" << endl;
    for (const string& prefix : prefixes) {
        trie.findWordsWithPrefix(prefix);
    }

    // iv. Return short words (a, an, by)
    cout << "\nSearching for short words:" << endl;
    vector<string> shortWords = {"a", "an", "by"};
    for (const string& word : shortWords) {
        trie.search(word);
    }

    // v. Return hyphenated words (e-mail, t-shirt)
    cout << "\nSearching for hyphenated words:" << endl;
    vector<string> hyphenatedWords = {"e-mail", "t-shirt"};
    for (const string& word : hyphenatedWords) {
        trie.search(word);
    }

    // vi. Return words with numbers (24hours, 2day)
    cout << "\nSearching for words with numbers:" << endl;
    vector<string> numberWords = {"24hours", "2day"};
    for (const string& word : numberWords) {
        trie.search(word);
    }

    // vii. Return mixed case words (JavaScript, PowerPoint)
    cout << "\nSearching for mixed case words:" << endl;
    vector<string> mixedCaseWords = {"JavaScript", "PowerPoint"};
    for (const string& word : mixedCaseWords) {
        trie.search(word);
    }

    // viii. Return special cases
    cout << "\nSearching for special case words:" << endl;
    vector<string> specialWords = {
        "psychology", "pterodactyl", "xylophone", "yacht",
        "eBay", "iPad", "x-ray"
    };
    for (const string& word : specialWords) {
        trie.search(word);
    }

    // ix. Handle exception and memory management (search for abc, xyz)
    cout << "\nHandling exceptions and memory management:" << endl;
    try {
        trie.search("abc");
        trie.search("xyz");
    } catch (const exception& e) {
        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}


// Time and Space Complexity Analysis

/*
1. Trie Operations:

c. Time complexity of operations in (i) and (ii):

   - (i) Inserting words into the trie: O(m), where m is the average length of the words.
     - For n words, the total time complexity is O(n * m).
   - (ii) Searching for a word in the trie: O(k), where k is the length of the word being searched.

d. Time complexity of operations in (iii) and (iv):

   - (iii) Finding words with a given prefix:
     - O(p + W), where p is the length of the prefix, and W is the total number of characters in all words with that prefix.
   - (iv) Searching for short words:
     - Same as searching for a word: O(k), where k is small since the words are short.

e. Space complexity of the trie of n words:

   - The space complexity is O(N * M), where N is the number of words and M is the average length of the words.
   - Each node can have up to 26 (or more, depending on the character set) children, but space is only allocated for characters that are used.
   - In the worst case, where all words are unique and share no common prefixes, space complexity is proportional to the sum of the lengths of 
   all words.

*/
