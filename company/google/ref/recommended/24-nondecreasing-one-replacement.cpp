// Longest Non-Decreasing Subarray With One Replacement (Google, interview report)
// Given an integer array nums, you may replace at most one element with any integer.
// Return the maximum length of a contiguous non-decreasing subarray afterwards
// (0 for an empty array).

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
}

// follow-up: strictly increasing, integer replacement -> use < instead of <= for left /
//   right; the runs join through i only if an integer fits strictly between the
//   neighbors: (long long)nums[i + 1] - nums[i - 1] >= 2
