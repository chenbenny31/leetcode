// 03 · Hangman, Next Letter (Moveworks phone screen, Medium) — filter + count
// given a dictionary, the board ("a__le", '_' = unknown) and every letter guessed so far,
// return the next letter to guess: the unguessed letter found in the most dictionary
// words that still fit the board
//
// clarify:
// statement known only from short reports -> this is the usual version; confirm it
// guessed includes the revealed letters and the wrong guesses -> yes
// a word fits -> same len, revealed spots equal, and no guessed letter at a '_' spot
//   (a guessed letter would have been revealed there)
// count words or occurrences -> words (a letter twice in a word still reveals in 1 guess)
// tie on words -> more total occurrences in the fitting words, then smallest letter
// no word fits -> return '\0'

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// T: O(len), S: O(1)
bool fits(const string& word, const string& board, const vector<bool>& guessed) {
    int len = board.size();
    if ((int)word.size() != len) { return false; }
    for (int i = 0; i < len; i++) {
        if (board[i] != '_' && word[i] != board[i]) { return false; }
        if (board[i] == '_' && guessed[word[i] - 'a']) { return false; }
    }
    return true;
}

// core idea: keep the words that fit; the letter in most of them reveals something
// most often
// T: O(W * L), S: O(1) — W = dictionary size, L = word len
char nextLetter(const vector<string>& dict, const string& board, const string& guesses) {
    // guessed[c] = letter c was already guessed (right or wrong)
    vector<bool> guessed(26, false);
    for (char c : guesses) { guessed[c - 'a'] = true; }

    // inWords[c] = num of fitting words that contain c
    vector<int> inWords(26, 0);
    // total[c] = occurrences of c across the fitting words (tie-break)
    vector<int> total(26, 0);
    for (const string& word : dict) {
        if (!fits(word, board, guessed)) { continue; }
        vector<bool> seen(26, false);
        for (char c : word) {
            seen[c - 'a'] = true;
            total[c - 'a']++;
        }
        for (int c = 0; c < 26; c++) {
            if (seen[c]) { inWords[c]++; }
        }
    }

    // -1 = no letter picked yet
    int best = -1;
    for (int c = 0; c < 26; c++) {
        if (guessed[c] || inWords[c] == 0) { continue; }
        if (best == -1 || inWords[c] > inWords[best]) {
            best = c;
        } else if (inWords[c] == inWords[best] && total[c] > total[best]) {
            // strict >, scanning a..z, keeps the smallest letter on a full tie
            best = c;
        }
    }
    if (best == -1) { return '\0'; }
    return 'a' + best;
}

void test(const string& label, const vector<string>& dict, const string& board,
          const string& guesses, char want) {
    cout << label << (nextLetter(dict, board, guesses) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("most-words", {"apple", "ample", "angle", "maple"}, "a___e", "ae", 'l');
    test("wrong-guess-filters", {"bat", "cat", "hat"}, "_at", "atc", 'b');
    test("tie-smallest", {"ab", "ac"}, "a_", "a", 'b');
    test("tie-by-occurrences", {"acc", "abd"}, "a__", "a", 'c');
    test("no-fit", {"cat", "dog"}, "_____", "", '\0');
    return 0;
}

// follow-up:
// play a whole game -> loop: guess, reveal (board or wrong list), stop at no '_' or
//   max misses
// better pick -> letter whose answer splits the fitting words most evenly (max entropy /
//   min expected worst case), not just the most common one
// huge dictionary, many games -> group words by len once; precompute per len
// guess the secret word w/ match feedback (LC 843, also reported) -> pick the word whose
//   worst-case group of same-match-count words is smallest, keep that group, repeat
