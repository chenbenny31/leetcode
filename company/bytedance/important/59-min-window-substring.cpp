// ByteDance onsite (FT), LC 76: return the shortest contiguous substring of s that
// contains every character of t with multiplicity (case-sensitive). If several have
// the shortest length, return the leftmost; if none exists, return "".
//   minCoveringSubstring(s, t) -> string
// Constraints: |s|, |t| <= 1e5, ASCII.
// Follow-up (discussed in the report): list the start of every shortest window. Keep
// the sweep below; for each right end the shrunk window is the shortest one ending
// there, so record its start whenever its length equals the best length so far (clear
// the list when a strictly shorter one appears).

#include <iostream>
#include <string>

using std::cout;
using std::string;

// Sliding window with counts. need[c] = how many more c the window still needs
// (negative = surplus); missing = total characters still needed. Grow the right end;
// once missing hits 0 the window covers t, so shrink from the left while the left
// character is surplus, then compare lengths. Only a strictly shorter window replaces
// the best, which keeps the leftmost among equal lengths. T: O(|s| + |t|), S: O(1).
string minCoveringSubstring(const string& s, const string& t) {
    if (t.empty()) {
        return "";
    }
    // need[c] = copies of c the window still lacks (negative = extra copies)
    int need[256] = {};
    for (unsigned char c : t) {
        need[c]++;
    }
    int missing = t.size();
    int bestStart = 0;
    int bestLen = -1;
    int left = 0;

    for (int right = 0; right < (int)s.size(); right++) {
        unsigned char c = s[right];
        if (need[c] > 0) {
            missing--;
        }
        need[c]--;

        if (missing > 0) {
            continue;
        }
        // drop left characters the window has more of than t needs
        while (need[(unsigned char)s[left]] < 0) {
            need[(unsigned char)s[left]]++;
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
    cout << label << (minCoveringSubstring(s, t) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("multiplicity", "abba", "aa", "abba");
    test("leftmost-tie", "xaybxab", "ab", "ab");
    test("case-sensitive", "aA", "A", "A");
    test("missing", "abc", "d", "");
    test("whole", "cab", "abc", "cab");
}
