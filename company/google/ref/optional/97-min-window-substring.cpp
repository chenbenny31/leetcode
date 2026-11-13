// 97 · Minimum Window Substring — LC 76
// Mother problem: select/optional/03-sliding-window/02-min-window-substring.cpp
//
// LC 76: return the shortest substring of s containing every character of t with
// multiplicity (case-sensitive ASCII), or "" if none.
//
// Clarify
// - duplicates in t? → must be covered with multiplicity
// - charset? → case-sensitive ASCII (128 slots); no window → ""
// - ties? → any shortest (the code keeps the first)
//
// Core idea
// - need[c] = copies the window still lacks (negative = surplus); `missing` = total
//   chars still lacking.
// - Grow right; once missing == 0, shrink left while need[s[left]] < 0, then record the
//   window.
// - T O(|s| + |t|): left and right each move at most |s| times. S O(1).
//
// Tests
// - multiplicity "abba", t "aa" → "abba"; shorter window later "xaybxab", t "ab" → "ab"
// - whole string "cab", t "abc"; missing char "abc", t "d" → ""
//
// Follow-ups
// - t as a subsequence (LC 727) → scan forward to match t, then backward from that end
//   for the latest start, O(|s| · |t|)

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
