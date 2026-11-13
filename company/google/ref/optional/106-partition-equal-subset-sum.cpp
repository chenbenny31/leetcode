// 106 · Partition Equal Subset Sum — LC 416
// Mother problem: select/13-dp/07-partition-equal-subset-sum.cpp
//
// Google reports: LC tag #45; winning electoral coalitions ×2 and task reward in a time
// budget ×2 (FastPrep, 0/1 knapsack)
//
// Clarify
// - values? → positive ints
// - both subsets non-empty? → a single element can never split, returns false
// - sum range? → total ≤ ~2e4, so a bool array over sums fits
//
// Core idea
// - Odd total → false; else some subset must reach half = total / 2.
// - dp[sum] = some subset of items seen so far adds to sum; dp[0] = true.
// - For each num, loop sum from half down to num so each item is used once.
//
// Tests: split, odd total, even but unreachable, single element
//
// Follow-ups
// - why loop downward → upward would reuse num in the same pass (unbounded knapsack /
//   coin change)
// - target sum (LC 494) → count subsets summing to (total + target) / 2 with dp[sum] +=
//   dp[sum − num]
// - winning coalitions → count subsets with sum > half; max reward in a budget → dp[t] =
//   max value
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Some subset must reach total / 2; 1D bool DP over sums. T: O(n * sum), S: O(sum).
bool canPartition(const vector<int>& nums) {
    int total = 0;
    for (int num : nums) {
        total += num;
    }
    if (total % 2 != 0) { return false; }
    int half = total / 2;
    // dp[sum] = some subset of the items seen so far adds up to sum
    vector<bool> dp(half + 1, false);
    dp[0] = true;
    for (int num : nums) {
        // high to low: dp[sum - num] is still from before num, so num is used once
        for (int sum = half; sum >= num; sum--) {
            if (dp[sum - num]) {
                dp[sum] = true;
            }
        }
    }
    return dp[half];
}

void test(const string& label, const vector<int>& nums, bool want) {
    cout << label << (canPartition(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("split", {3, 1, 1, 3}, true);
    test("odd-total", {1, 2, 4}, false);
    test("unreachable", {1, 1, 6}, false);
    test("single", {2}, false);
}
