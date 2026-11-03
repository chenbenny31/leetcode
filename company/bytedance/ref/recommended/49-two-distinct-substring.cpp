// ByteDance phone (FT), LC 159: return the length of the longest contiguous substring
// of s with at most two distinct characters. An empty string gives 0.
//   longestTwoDistinct(s) -> int
// Constraints: |s| <= 5e4, lowercase letters.

#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::max;

// Sliding window: extend the right end one character at a time; while the window has
// more than two distinct characters, shrink it from the left. The window is always
// the longest valid one ending at `right`. T: O(n), S: O(1) (26 counters).
int longestTwoDistinct(const string& s) {
    // count[c] = occurrences of letter c inside the window
    int count[26] = {};
    int distinct = 0;
    int best = 0;
    int left = 0;
    for (int right = 0; right < (int)s.size(); right++) {
        int c = s[right] - 'a';
        if (count[c] == 0) {
            distinct++;
        }
        count[c]++;

        while (distinct > 2) {
            int d = s[left] - 'a';
            count[d]--;
            if (count[d] == 0) {
                distinct--;
            }
            left++;
        }
        best = max(best, right - left + 1);
    }
    return best;
}

void test(const string& label, const string& s, int want) {
    cout << label << (longestTwoDistinct(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("empty", "", 0);
    test("one-letter", "zzz", 3);
    test("shrink", "abaccc", 4);
    test("all-different", "abcd", 2);
}
