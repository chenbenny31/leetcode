// Longest Non-Decreasing Subarray With One Replacement (Google, interview report)
// may replace at most one element w/ any int; return max len of a non-decreasing subarray
//
// clarify:
// new value -> any int
// zero replacements -> allowed
// equal neighbors -> fine (<=)
// n -> up to ~1e5, want O(n)
// empty array -> 0; return len only

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// len of longest non-decreasing run in nums
int longestRun(const vector<int>& nums) {
    int n = nums.size();
    int best = 0;
    int run = 0;
    for (int i = 0; i < n; i++) {
        if (i > 0 && nums[i - 1] <= nums[i]) {
            run++;
        } else {
            run = 1;
        }
        best = max(best, run);
    }
    return best;
}

// solution 1: brute force; state it first, use as a checker
// core idea: best new value for nums[i] copies a neighbor; try both, rescan longest run
// T: O(n^2), S: O(1)
int longestOneReplace(vector<int> nums) {
    int n = nums.size();
    int best = longestRun(nums);
    for (int i = 0; i < n; i++) {
        int original = nums[i];
        if (i > 0) {
            nums[i] = nums[i - 1];
            best = max(best, longestRun(nums));
        }
        if (i + 1 < n) {
            nums[i] = nums[i + 1];
            best = max(best, longestRun(nums));
        }
        nums[i] = original;
    }
    return best;
}

// solution 2: left / right run lengths; the O(n) answer
// core idea: precompute run ending at / starting at every idx
// replacing nums[i] extends a neighbor run by 1; joins both if nums[i-1] <= nums[i+1]
// T: O(n), S: O(n)
int longestOneReplaceFast(const vector<int>& nums) {
    int n = nums.size();
    if (n <= 1) { return n; }
    // left[i] = len of non-decreasing run ending at i
    vector<int> left(n, 1);
    // right[i] = len of non-decreasing run starting at i
    vector<int> right(n, 1);
    for (int i = 1; i < n; i++) {
        if (nums[i - 1] <= nums[i]) { left[i] = left[i - 1] + 1; }
    }
    for (int i = n - 2; i >= 0; i--) {
        if (nums[i] <= nums[i + 1]) { right[i] = right[i + 1] + 1; }
    }
    int best = 1;
    for (int i = 0; i < n; i++) {
        if (i > 0) { best = max(best, left[i - 1] + 1); }
        if (i + 1 < n) { best = max(best, right[i + 1] + 1); }
        // some value fits between both neighbors -> the two runs join through i
        if (i > 0 && i + 1 < n && nums[i - 1] <= nums[i + 1]) {
            best = max(best, left[i - 1] + 1 + right[i + 1]);
        }
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    bool ok = longestOneReplace(nums) == want && longestOneReplaceFast(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("sorted", {1, 2, 3}, 3);
    test("no-join", {3, 4, 1, 2}, 3);
    test("join-equal", {2, 0, 2, 3}, 4);
    test("empty", {}, 0);
}

// follow-up:
// strictly increasing, int replacement -> < for left / right; join only if gap >= 2
// gap = nums[i+1] - nums[i-1] -> compute in long long
// return idx + new value -> record where best grows: nums[i-1] (left, join), nums[i+1]
// delete exactly one instead (as in LC 1493) -> deleted cell adds no len, drop the + 1
// whole array fixable w/ one change (LC 665) -> answer == n
// LC 665, O(1) space -> on a drop lower nums[i-1] if nums[i-2] allows, else raise nums[i]
// LC 665, second drop -> false
