// LC 416 Partition Equal Subset Sum (Medium) — 0/1 knapsack, subset sum
// nums are positive; can they be split into two subsets with equal sums?
// Clarify: values positive; every element goes to one of the two subsets; total small
//   enough for an O(total) table; single element is false.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: an odd total fails; otherwise ask whether some subset reaches total / 2,
// with a 1D bool DP over sums filled from high to low. T: O(n * sum), S: O(sum).
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

// follow-up (ideas only):
// - why loop sum downward -> upward would reuse num in the same pass, which is the
//   unbounded knapsack (coin change)
// - target sum with +/- signs (LC 494) -> count subsets summing to (total + target) / 2
//   with dp[sum] += dp[sum - num], same downward loop
// - coin change II (LC 518) -> unbounded: coins outside, sum upward, dp[sum] +=
//   dp[sum - coin] counts combinations
// - last stone weight II (LC 1049) -> largest reachable subset sum s <= total / 2 with
//   the same dp; answer = total - 2 * s
