// LC 76 Minimum Window Substring (Hard). Pattern: variable window, need counts +
// missing counter.
// Return the shortest substring of s containing every character of t with multiplicity
// (case-sensitive ASCII), or "" if none.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Grow right until the window covers t, then shrink left past surplus chars.
// T: O(|s| + |t|), S: O(1) (128 slots).
string minWindow(const string& s, const string& t) {
    // need[c] = copies of c the window still lacks (negative = extra copies)
    vector<int> need(128, 0);
    for (char c : t) {
        need[c]++;
    }
    // chars of t the window still lacks; 0 = window covers t
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
        // drop left chars the window has more of than t needs
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

// follow-up: why O(n) -> right and left each move at most |s| times
// follow-up: t as subsequence (LC 727) -> scan forward to match t, then backward from
//   that end for the latest start; O(|s| * |t|)
