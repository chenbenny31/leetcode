// LC 408 Valid Word Abbreviation: abbr replaces any number of non-adjacent substrings of
// word by their length (no leading zeros, no empty substring). Is abbr valid for word?
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Two pointers: a letter must match, a number skips that many letters of word. Both
// strings must end together. T: O(n + m), S: O(1).
bool validWordAbbreviation(const string& word, const string& abbr) {
    int n = word.size();
    int m = abbr.size();
    int i = 0;   // position in word
    int j = 0;   // position in abbr
    while (i < n && j < m) {
        if (abbr[j] < '0' || abbr[j] > '9') {
            if (word[i] != abbr[j]) { return false; }
            i++;
            j++;
            continue;
        }
        if (abbr[j] == '0') { return false; }
        int skip = 0;
        while (j < m && abbr[j] >= '0' && abbr[j] <= '9') {
            skip = skip * 10 + (abbr[j] - '0');
            // stops int overflow on long digit runs; skipping past the end never matches
            if (skip > n) { return false; }
            j++;
        }
        i += skip;
    }
    return i == n && j == m;
}

void test(const string& label, const string& word, const string& abbr, bool want) {
    cout << label << (validWordAbbreviation(word, abbr) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("letters-and-number", "internal", "i6l", true);
    test("number-only", "word", "4", true);
    test("overshoot", "word", "w4d", false);
    test("leading-zero", "word", "w02d", false);
}
