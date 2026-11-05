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

// Sliding window: grow right; while > 2 distinct, shrink left. T: O(n), S: O(1).
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

// follow-up: at most k distinct (LC 340) -> same window, shrink while distinct > k, O(n)
// follow-up: return substr -> bestStart = left on improve, s.substr(bestStart, best)
// follow-up: any charset -> unordered_map<char, int> count, erase at 0, distinct = size()
// follow-up: # subarrays with exactly k distinct (LC 992) -> atMost(k) - atMost(k - 1),
//   atMost adds right - left + 1 per step, O(n)
