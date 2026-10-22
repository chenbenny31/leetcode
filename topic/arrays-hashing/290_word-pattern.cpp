// two hash-maps, T: O(m + n) - n = pattern len, m = total chars, S: O(m + n)

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream> // std::istringstream

class Solution {
public:
    bool wordPattern(std::string pattern, std::string s) {
        std::vector<std::string> words;
        std::istringstream iss(s);
        std::string w;
        while (iss >> w) { words.push_back(w); } // >> skips runs of whitespace

        const int n = static_cast<int>(pattern.length());
        if (n != static_cast<int>(words.size())) { return false; }

        std::unordered_map<char, std::string> charToWord;
        std::unordered_map<std::string, char> wordToChar;

        for (int i = 0; i < n; i++) {
            char c = pattern[i];
            const std::string& word = words[i];

            auto itc = charToWord.find(c);
            if (itc != charToWord.end()) {
                if (itc->second != word) { return false; }
            } else {
                charToWord[c] = word;
            }

            auto itw = wordToChar.find(word);
            if (itw != wordToChar.end()) {
                if (itw->second != c) { return false; }
            } else {
                wordToChar[word] = c;
            }
        }
        return true;
    }
};

// first-occurrence index, T: (m + n), S: O(n + m)

class Solution {
public:
    bool wordPattern(std::string pattern, std::string s) {
        std::vector<std::string> words;
        std::istringstream iss(s);
        std::string w;
        while (iss >> w) { words.push_back(w); }

        const int n = static_cast<int>(pattern.length());
        if (n != static_cast<int>(words.size())) { return false; }

        std::unordered_map<char, int> charFirst;
        std::unordered_map<std::string, int> wordFirst;

        for (int i = 0; i < n; i++) {
            // emplace does nothing if key exists
            auto a = charFirst.emplace(pattern[i], i); // emplace returns {iterator, bool}
            auto b = wordFirst.emplace(words[i], i);
            if (a.first->second != b.first->second) { return false; } // iterator = {key, value}
        }
        return true;
    }
};
