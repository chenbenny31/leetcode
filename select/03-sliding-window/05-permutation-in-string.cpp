// LC 567 Permutation in String (Medium). Pattern: fixed-size window of letter counts.
// Return true if some permutation of s1 is a substring of s2 (both lowercase).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Slide a window of size |s1| over s2; a permutation iff its letter counts equal s1's.
// T: O(26 * n), S: O(1).
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
}

// follow-up: all start indices (LC 438) -> same loop, push i - m + 1 when equal
// follow-up: strict O(n) -> keep matches = letters whose counts agree; update it only
//   for the entering and leaving letter, permutation iff matches == 26
