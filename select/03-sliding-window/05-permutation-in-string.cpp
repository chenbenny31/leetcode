// LC 567 Permutation in String (Medium) — fixed-size window of letter counts
// return true if some permutation of s1 is a substring of s2
//
// clarify:
// charset -> lowercase only, 26 slots
// s1 longer than s2 -> false
// empty s1 -> assume non-empty
// repeated letters -> cnts must match, not just the set

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: slide window of size |s1| over s2; permutation iff letter cnts = s1's
// T: O(26 * n), S: O(1)
bool checkInclusion(const string& s1, const string& s2) {
    int m = s1.size();
    int n = s2.size();
    // need[c] = cnt of letter 'a' + c in s1
    vector<int> need(26, 0);
    for (char c : s1) {
        need[c - 'a']++;
    }
    // window[c] = cnt of letter 'a' + c in s2[i-m+1..i]
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

// follow-up:
// all anagram start idx (LC 438) -> same window; record i - m + 1 on every match
// strict O(n) -> matches = letters whose two cnts agree; update only entering / leaving
