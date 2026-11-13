// 102 · Minimum Window Substring — LC 76
// Mother: select/optional/03-sliding-window/02-min-window-substring.cpp
// shortest substring of s containing every char of t w/ multiplicity; "" if none
//
// clarify:
// dups in t -> each copy must be covered
// charset -> case-sensitive ASCII, 128 slots
// ties -> any window (this returns the first)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: grow right until window covers t, shrink left past surplus, record window
// T: O(|s| + |t|), S: O(1)
string minWindow(const string& s, const string& t) {
    // need[c] = copies of c window still lacks (negative = extra)
    vector<int> need(128, 0);
    for (char c : t) {
        need[c]++;
    }
    // chars of t window still lacks; 0 = covers t
    int missing = t.size();
    int bestStart = 0;
    int bestLen = -1;
    int left = 0;
    int n = s.size();
    for (int right = 0; right < n; right++) {
        char c = s[right];
        if (need[c] > 0) {
            missing--;
        }
        need[c]--;
        if (missing > 0) {
            continue;
        }
        // drop left chars the window has in surplus
        while (need[s[left]] < 0) {
            need[s[left]]++;
            left++;
        }
        int len = right - left + 1;
        if (bestLen == -1 || len < bestLen) {
            bestLen = len;
            bestStart = left;
        }
    }
    if (bestLen == -1) {
        return "";
    }
    return s.substr(bestStart, bestLen);
}

void test(const string& label, const string& s, const string& t, const string& want) {
    cout << label << (minWindow(s, t) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("multiplicity", "abba", "aa", "abba");
    test("shorter-later", "xaybxab", "ab", "ab");
    test("whole", "cab", "abc", "cab");
    test("missing", "abc", "d", "");
}

// follow-up:
// t as subsequence (LC 727) -> scan forward to match t, backward for latest start
