// LC 53 Maximum Subarray (Medium) — Kadane: best sum ending here
// Return the largest sum of a non-empty contiguous subarray of nums.
// Clarify: empty input; all-negative input (the answer is the largest element, not 0);
//   overflow of the sum; return the sum or the subarray itself.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Core idea: the best subarray ending at x either extends the best one ending just
// before it or starts fresh at x: ending = max(x, ending + x). T: O(n), S: O(1).
int maxSubArray(const vector<int>& nums) {
    if (nums.empty()) { return 0; }
    // best sum of a subarray that ends exactly at the current element
    int ending = nums[0];
    int best = nums[0];
    for (int i = 1; i < (int)nums.size(); i++) {
        ending = max(nums[i], ending + nums[i]);
        best = max(best, ending);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (maxSubArray(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle-run", {-2, 3, -1, 4, -5}, 6);
    test("all-negative", {-3, -1, -2}, -1);
    test("whole-array", {1, 2, 3}, 6);
    test("single", {5}, 5);
    return 0;
}

// follow-up (ideas only):
// - return the subarray indices -> remember where the current run started (reset it when
//   starting fresh at x); copy (start, i) whenever best improves
// - circular array (LC 918) -> max(kadane, total - min subarray); if every value is
//   negative the second term is the empty array, so return kadane
// - max product subarray (LC 152) -> track both max and min product ending here, since a
//   negative x swaps them
// - max alternating subsequence sum (LC 1911) -> two states: best sum whose last pick has
//   an even index (added) / an odd index (subtracted)
// - at most one deletion (LC 1186) -> two states: best ending here with no deletion, and
//   with one deletion = max(noDelete before x, oneDelete + x)
// - 2D max sum submatrix -> fix a top and a bottom row, keep the column sums between
//   them, run Kadane on that 1D array: O(rows^2 * cols)
