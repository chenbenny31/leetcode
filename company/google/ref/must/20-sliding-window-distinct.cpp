// 20 · Longest Substring Without Repeating Chars (Sliding Window) — LC 3, 340, 904, 992
// Mother: select/03-sliding-window/01-longest-substring-no-repeat.cpp
// (1) LC 3: len of longest substring of s w/ no repeated char
// (2) shortest subarray of ints w/ >= k distinct values; -1 if none
//
// clarify:
// alphabet -> ASCII, 128 slots
// empty string -> 0
// space -> a char like any other
// (2) k <= 0 -> 0 (empty subarray)
// (2) fewer than k distinct in total -> -1

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

// (1) longest substring w/o repeat (LC 3)
// core idea: window has no repeat; on a repeat, jump left past that char's last idx
// T: O(n), S: O(1)
int lengthOfLongestSubstring(const string& s) {
    // last[c] = latest idx of char c, -1 = not seen
    vector<int> last(128, -1);
    int best = 0;
    int left = 0;
    int n = s.size();
    for (int right = 0; right < n; right++) {
        char c = s[right];
        // max: a stale last idx (left of the window) must not move left backwards
        left = max(left, last[c] + 1);
        last[c] = right;
        best = max(best, right - left + 1);
    }
    return best;
}

// (2) shortest subarray w/ >= k distinct
// core idea: extend right; while window has >= k distinct, record len and shrink left
// T: O(n), S: O(n)
int shortestAtLeastKDistinct(const vector<int>& nums, int k) {
    // empty subarray has 0 distinct (and the while would run past right)
    if (k <= 0) { return 0; }
    // count[num] = occurrences of num in the window
    unordered_map<int, int> count;
    // num of values w/ count > 0 in the window
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

// follow-up:
// at most k distinct (LC 340) -> cnt map + distinct cnt; while distinct > k, shrink left
// fruit into baskets (LC 904) -> at most k distinct w/ k = 2
// exactly k distinct (LC 992) -> countAtMost(k) - countAtMost(k - 1)
// O(1) "[l, r] has no dup?" queries (range duplicate check) -> 21
// return the substring -> also keep bestStart
// int alphabet / any byte -> unordered_map last / 256 slots via (unsigned char)
