// Longest Non-Decreasing Subarray With One Replacement (Google, interview report)
// Given an integer array nums, you may replace at most one element with any integer.
// Return the maximum length of a contiguous non-decreasing subarray afterwards
// (0 for an empty array).
//
// Clarify:
// - the new value may be any integer (stated) -> it can always copy a neighbor
// - "at most one" -> zero replacements allowed: a sorted array gives n
// - non-decreasing -> equal neighbors are fine (<=)
// - n up to ~1e5 -> O(n) with run arrays; the O(n^2) try-every-index is the check
// - empty -> 0; one element -> 1
// - return the length only (the index and value to use are a follow-up)
// Idea: replacing nums[i] can extend the run ending at i - 1 by one (copy nums[i - 1]),
//   extend the run starting at i + 1 by one (copy nums[i + 1]), or join both runs when
//   nums[i - 1] <= nums[i + 1] (any value between them). left[] and right[] hold those
//   runs, so each i is O(1).
// Pitfalls:
// - check i > 0 before left[i - 1] / nums[i - 1] and i + 1 < n before right[i + 1] /
//   nums[i + 1]: index -1 and n at the two ends
// - the join compares nums[i - 1] with nums[i + 1], not with nums[i] (it is replaced)
// - n == 0 returns before best = 1; no replacement at all is covered by the one-side
//   extensions (a sorted array gets right[1] + 1 = n at i = 0)
// - brute force: restore nums[i] after both tries, or later tries see two replacements
// Tests to write: sorted -> n; a bump joined; two runs that cannot join; the drop at the
//   first / last index; equal neighbors join; one element; empty.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// length of the longest non-decreasing run in nums
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

// straightforward: the best new value for nums[i] copies a neighbor (the left one extends
// the left run as far as possible, the right one the right run); try both for every i.
// T: O(n^2), S: O(1).
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

// improved: precompute the run ending at / starting at every index; replacing nums[i]
// extends either neighbor run by one, and joins both when nums[i-1] <= nums[i+1].
// T: O(n), S: O(n).
int longestOneReplaceFast(const vector<int>& nums) {
    int n = nums.size();
    if (n <= 1) { return n; }
    // left[i] = length of the non-decreasing run ending at i
    vector<int> left(n, 1);
    // right[i] = length of the non-decreasing run starting at i
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
        // some value fits between both neighbors: the two runs join through i
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
    test("one-bump", {1, 5, 2, 3}, 4);
    test("no-join", {3, 4, 1, 2}, 3);
    test("first-drop", {9, 1, 2}, 3);
    test("single", {5}, 1);
    test("last-drop", {1, 2, 0}, 3);
    test("join-equal", {2, 0, 2, 3}, 4);
    test("empty", {}, 0);
}

// follow-up: strictly increasing, integer replacement -> use < instead of <= for left /
//   right; the runs join through i only if an integer fits strictly between the
//   neighbors: (long long)nums[i + 1] - nums[i - 1] >= 2 (the int difference overflows)
// follow-up: return which index to replace and its new value -> record them wherever
//   best grows: i with nums[i - 1] for the left run and the join, nums[i + 1] for the
//   right run
// follow-up: delete exactly one element instead (as in LC 1493) -> the deleted cell adds
//   no length: candidates left[i - 1], right[i + 1] and, when nums[i - 1] <= nums[i + 1],
//   left[i - 1] + right[i + 1]
// follow-up: whole array fixable with one change (LC 665) ->
//   longestOneReplaceFast(nums) == (int)nums.size(); O(1) space greedy: at the first
//   drop nums[i - 1] > nums[i], lower nums[i - 1] to nums[i] if i < 2 or nums[i - 2] <=
//   nums[i], else raise nums[i] to nums[i - 1]; a second drop -> false
