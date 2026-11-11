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

// LC 438: same size-|p| window over s; push its start index whenever counts match.
// T: O(26 * n), S: O(1) + output.
vector<int> findAnagrams(const string& s, const string& p) {
    int m = p.size();
    int n = s.size();
    // need[c] = count of letter 'a' + c in p
    vector<int> need(26, 0);
    for (char c : p) {
        need[c - 'a']++;
    }
    // window[c] = count of letter 'a' + c in s[i-m+1..i]
    vector<int> window(26, 0);
    // start indices of windows that are anagrams of p
    vector<int> starts;
    for (int i = 0; i < n; i++) {
        window[s[i] - 'a']++;
        if (i >= m) {
            window[s[i - m] - 'a']--;
        }
        if (window == need) {
            starts.push_back(i - m + 1);
        }
    }
    return starts;
}

void test(const string& label, const string& s1, const string& s2, bool want) {
    cout << label << (checkInclusion(s1, s2) == want ? " PASS\n" : " FAIL\n");
}

void testAnagrams(const string& label, const string& s, const string& p,
                  const vector<int>& want) {
    cout << label << (findAnagrams(s, p) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", "ab", "xbay", true);
    test("at-end", "abc", "xxcab", true);
    test("count-differs", "aab", "abba", false);
    test("too-long", "abc", "ab", false);
    testAnagrams("all-overlapping", "baab", "ab", {0, 2});
    testAnagrams("all-start-and-end", "abcxcab", "abc", {0, 4});
    testAnagrams("all-none", "abba", "aab", {});
}

// follow-up: strict O(n) -> keep matches = letters whose counts agree; update it only
//   for the entering and leaving letter, permutation iff matches == 26
