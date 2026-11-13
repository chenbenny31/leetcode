// LC 567 Permutation in String (Medium) — fixed-size window of letter counts
// Return true if some permutation of s1 is a substring of s2.
// Clarify: alphabet (lowercase only); s1 longer than s2; empty s1; repeated letters.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: slide a window of size |s1| over s2; it is a permutation exactly when its
// letter counts equal those of s1. T: O(26 * n), S: O(1).
bool checkInclusion(const string& s1, const string& s2) {
    int m = s1.size();
    int n = s2.size();
    // need[c] = count of letter 'a' + c in s1
    vector<int> need(26, 0);
    for (char c : s1) {
        need[c - 'a']++;
    }
    // window[c] = count of letter 'a' + c in s2[i-m+1..i]
    vector<int> window(26, 0);
    for (int i = 0; i < n; i++) {
        window[s2[i] - 'a']++;
        // s2[i - m] just left the size-m window
        if (i >= m) {
            window[s2[i - m] - 'a']--;
        }
        if (window == need) {
            return true;
        }
    }
    return false;
}

void test(const string& label, const string& s1, const string& s2, bool want) {
    cout << label << (checkInclusion(s1, s2) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", "ab", "xbay", true);
    test("at-end", "abc", "xxcab", true);
    test("count-differs", "aab", "abba", false);
    test("too-long", "abc", "ab", false);
    return 0;
}

// follow-up (ideas only):
// - all anagram start indices (LC 438) -> same window; instead of returning, record
//   i - m + 1 every time the counts match
// - strict O(n) -> keep matches = letters whose two counts agree; update it only for the
//   entering and the leaving letter; a permutation when matches == 26
