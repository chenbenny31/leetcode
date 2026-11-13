// LC 3 Longest Substring Without Repeating Characters (Medium) — variable window, jump
// left past the last seen index
// Return the length of the longest substring of s with no repeated character.
// Clarify: character set (ASCII, 128 slots); substring is contiguous; a space is a
//   character; empty string -> 0; return the length, not the substring.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Core idea: the window s[left..right] never holds a repeat; when s[right] was last seen
// inside it, jump left to just past that index. T: O(n), S: O(1) (128 slots).
int lengthOfLongestSubstring(const string& s) {
    // last[c] = latest index of char c, -1 = not seen yet
    vector<int> last(128, -1);
    int best = 0;
    int left = 0;
    int n = s.size();
    for (int right = 0; right < n; right++) {
        char c = s[right];
        // max: a last index left of the window must not move left backwards
        left = max(left, last[c] + 1);
        last[c] = right;
        best = max(best, right - left + 1);
    }
    return best;
}

void test(const string& label, const string& s, int want) {
    cout << label << (lengthOfLongestSubstring(s) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("repeat-at-start", "abcad", 4);
    test("all-same", "aaa", 1);
    test("stale-index", "abba", 2);
    test("empty", "", 0);
    return 0;
}

// follow-up (ideas only):
// - return the substring itself -> record bestStart = left whenever best improves
// - large alphabet or ints -> last becomes a hash map (missing key = not seen); bytes
//   above 127 need 256 slots indexed by (unsigned char)c
// - longest with at most k distinct (LC 340; LC 904 fruit baskets is k = 2) -> count per
//   char plus a distinct counter; shrink left one step at a time while distinct > k
// - shortest subarray with at least k distinct -> while distinct >= k, record the
//   length and then shrink left; -1 if none, 0 when k <= 0
// - count subarrays with exactly k distinct (LC 992) -> atMost(k) - atMost(k - 1), where
//   atMost adds right - left + 1 for every right
// - offline queries "is nums[l..r] duplicate-free" -> store leftmost[r] = the window's
//   left for each right; the answer is l >= leftmost[r]
