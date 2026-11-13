// 19 · Longest Substring Without Repeating Characters (Sliding Window) — LC 3, 340, 904,
//   992
// Mother problem: select/03-sliding-window/01-longest-substring-no-repeat.cpp
//
// Google reports: shortest with at least k distinct ×9 (`shortestAtLeastKDistinct`), at
// most 2 distinct (LC 904), range uniqueness queries ×2
//
// LC 3: length of the longest substring of s (ASCII) with no repeated character.
//
// Clarify
// - ASCII → 128 slots; any byte → 256 slots via (unsigned char)
// - substring = contiguous; a space is a character; empty string → 0
//
// Core idea
// - Window s[left..right] never holds a repeat; on a repeat, jump left past that char's
//   last index: left = max(left, last[c] + 1), the max skips stale indices ("abba").
// - T O(n), S O(1) (128 slots).
//
// Follow-ups
// - shortest with at least k distinct (Google ×9) → `shortestAtLeastKDistinct` below
// - at most k distinct (LC 340) → count array + distinct counter; while distinct > k,
//   shrink left one step at a time
// - fruit into baskets (LC 904) → at most k distinct with k = 2 and an int count map
// - exactly k distinct (LC 992) → countAtMost(k) - countAtMost(k - 1), where countAtMost
//   adds right - left + 1 per right end
// - O(1) range "no duplicate" queries → store leftmost[r] = the window's left at r;
//   [l, r] is unique iff l >= leftmost[r]
// - return the substring → also keep bestStart; int alphabet → unordered_map last

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::max;

// Window s[left..right] has no repeats; on a repeat, jump left past that char's last
// index. T: O(n), S: O(1) (128 slots).
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

// Shortest subarray with at least k distinct ints. Extend right; while the window has
// >= k distinct, record its length and shrink left. -1 if none. T: O(n), S: O(n).
// Pitfalls: record the length inside the while, before shrinking; k <= 0 -> 0 up front,
//   else the while shrinks past right and reads nums[n].
int shortestAtLeastKDistinct(const vector<int>& nums, int k) {
    // the empty subarray already has 0 distinct values
    if (k <= 0) { return 0; }
    // count[num] = occurrences of num in the window
    unordered_map<int, int> count;
    // number of values with count > 0 in the window
    int distinct = 0;
    int best = -1;
    int left = 0;
    int n = nums.size();
    for (int right = 0; right < n; right++) {
        int num = nums[right];
        if (count[num] == 0) {
            distinct++;
        }
        count[num]++;
        while (distinct >= k) {
            int len = right - left + 1;
            if (best == -1 || len < best) {
                best = len;
            }
            int out = nums[left];
            count[out]--;
            if (count[out] == 0) {
                distinct--;
            }
            left++;
        }
    }
    return best;
}

void test(const string& label, const string& s, int want) {
    cout << label << (lengthOfLongestSubstring(s) == want ? " PASS\n" : " FAIL\n");
}

void testAtLeastK(const string& label, const vector<int>& nums, int k, int want) {
    bool ok = shortestAtLeastKDistinct(nums, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("repeat-at-start", "abcad", 4);
    test("all-same", "aaa", 1);
    test("stale-index", "abba", 2);
    test("empty", "", 0);
    testAtLeastK("at-least-later-shorter", {1, 2, 1, 1, 3, 2}, 3, 3);
    testAtLeastK("at-least-none", {2, 2, 2}, 2, -1);
    testAtLeastK("at-least-whole", {1, 2, 3}, 3, 3);
    testAtLeastK("at-least-k-zero", {4}, 0, 0);
}
