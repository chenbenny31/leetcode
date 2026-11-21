// 04 · Max Coins Summing To Target (Cisco OA, Medium) — 0/1 knapsack, max count
// Mother: select/13-dp/07-partition-equal-subset-sum.cpp
// each coin used at most once; return max num of coins w/ sum exactly target, -1 if none
//
// clarify:
// coin values -> positive ints
// bounds on target / values -> the OA left them out; ask. DP needs target <= ~1e6
// target == 0 -> 0 coins
// same value twice in the input -> two separate coins

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// core idea: dp over sums; taking a coin adds 1 to the best count of (sum - coin)
// T: O(n * target), S: O(target)
int maxCoins(const vector<int>& coins, int target) {
    // dp[s] = max coins w/ sum exactly s, -1 = unreachable
    vector<int> dp(target + 1, -1);
    dp[0] = 0;
    for (int coin : coins) {
        // downward, so dp[s - coin] is still the value w/o this coin
        for (int s = target; s >= coin; s--) {
            if (dp[s - coin] != -1) {
                dp[s] = max(dp[s], dp[s - coin] + 1);
            }
        }
    }
    return dp[target];
}

void test(const string& label, const vector<int>& coins, int target, int want) {
    cout << label << (maxCoins(coins, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("small-coins-win", {1, 2, 3, 4}, 6, 3);
    test("duplicates", {5, 5, 5}, 10, 2);
    test("unreachable", {2, 4}, 5, -1);
    test("zero", {3}, 0, 0);
    return 0;
}

// follow-up:
// why not greedy (smallest first) -> {1, 2, 4}, target 5: takes 1 + 2, answer is 1 + 4
// target huge, n <= 20 -> try all 2^n subsets w/ a bitmask
// target huge, n <= 40 -> meet in the middle: sum -> max count per half, match the halves
// min num of coins, unlimited use (LC 322) -> select/13-dp/02-coin-change (sum upward)
// which coins -> keep took[i][s], walk back from target
