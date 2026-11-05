// ByteDance onsite (FT), LC 76: return the shortest substring of s containing every
// character of t with multiplicity (case-sensitive, ASCII). Ties -> leftmost; none -> "".

#include <iostream>
#include <string>

using std::cout;
using std::string;

// Sliding window: grow right until it covers t, then shrink left past surplus chars.
// T: O(|s| + |t|), S: O(1).
string minCoveringSubstring(const string& s, const string& t) {
    if (t.empty()) {
        return "";
    }
    // need[c] = copies of c the window still lacks (negative = extra copies)
    int need[256] = {};
    for (unsigned char c : t) {
        need[c]++;
    }
    // characters of t the window still lacks; 0 = window covers t
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
        // strictly shorter only: keeps the leftmost among equal lengths
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

// follow-up: starts of all shortest windows -> after shrinking: len < bestLen clears
//   starts, len == bestLen pushes left; still O(|s|)
// follow-up: t as subsequence (LC 727) -> scan forward to match t, then backward from
//   that end for the latest start; O(|s| * |t|)
// follow-up: unicode / int elements -> unordered_map<int, int> need, same logic
