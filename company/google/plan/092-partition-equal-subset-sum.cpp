// LC 416 Partition Equal Subset Sum (Medium) — 0/1 knapsack, subset sum
// return if positive nums split into two subsets w/ equal sums
//
// clarify:
// values positive -> sums only grow, dp idx >= 0
// every element goes to one of the two subsets -> find a subset = total / 2
// total small -> O(total) table fits
// single element -> false

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: odd total fails; else can some subset reach total / 2? 1D bool DP over sums
// T: O(n * sum), S: O(sum)
bool canPartition(const vector<int>& nums) {
    int total = 0;
    for (int num : nums) {
        total += num;
    }
    if (total % 2 != 0) { return false; }
    int half = total / 2;
    // dp[sum] = some subset of items seen so far adds up to sum
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

// follow-up:
// why loop sum downward -> upward reuses num in the same pass = unbounded knapsack
// target sum w/ +/- signs (LC 494) -> cnt subsets summing to (total + target) / 2
// coin change II (LC 518) -> unbounded: coins outside, sum upward; cnts combos
// last stone weight II (LC 1049) -> max reachable s <= total / 2; answer = total - 2 * s
