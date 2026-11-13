// 106 · Partition Equal Subset Sum — LC 416
// Mother: select/13-dp/07-partition-equal-subset-sum.cpp
// can nums (positive ints) be split into 2 subsets w/ equal sums?
// reported: LC tag #45; winning electoral coalitions x2; task reward in a budget x2
//
// clarify:
// values -> positive
// single element -> cannot split
// total size -> small enough for a bool array over sums (~2e4)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: odd total fails; else some subset must reach total / 2
// 1D bool DP over sums, looping sums downward so each item is used once
// T: O(n * sum), S: O(sum)
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

// follow-up:
// why loop downward -> upward would reuse num in the same pass (unbounded knapsack)
// target sum (LC 494) -> count subsets w/ sum (total + target) / 2; dp[sum] counts ways
// winning coalitions (Google) -> count subsets w/ sum > half by the counting DP
// max reward in a time budget (Google) -> 0/1 knapsack: dp[t] = max reward within time t
