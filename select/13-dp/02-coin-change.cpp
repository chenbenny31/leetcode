// LC 322 Coin Change (Medium) — unbounded knapsack DP
// Unlimited coins of each denomination; return the fewest coins that sum to amount,
// or -1 if no combination works.
// Clarify: coins positive; unlimited supply; amount 0 returns 0; unreachable returns
//   -1; amount small enough for an O(amount) table.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// Core idea: dp[sum] = 1 + min over coins of dp[sum - coin], built from sum 1 up.
// T: O(amount * coins), S: O(amount).
int coinChange(const vector<int>& coins, int amount) {
    // dp[sum] = fewest coins adding up to sum; amount + 1 acts as infinity
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    for (int sum = 1; sum <= amount; sum++) {
        for (int coin : coins) {
            if (coin <= sum) {
                dp[sum] = min(dp[sum], dp[sum - coin] + 1);
            }
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}

void test(const string& label, const vector<int>& coins, int amount, int want) {
    cout << label << (coinChange(coins, amount) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("greedy-fails", {1, 3, 4}, 6, 2);
    test("mixed", {2, 5}, 11, 4);
    test("impossible", {5, 10}, 3, -1);
    test("zero-amount", {1}, 0, 0);
}

// follow-up (ideas only):
// - count the combinations (LC 518) -> dp[0] = 1, dp[sum] += dp[sum - coin] with coins
//   in the OUTER loop, so each multiset is built in coin order and counted once
// - count ordered sequences (LC 377) -> same sum, but amounts in the outer loop, which
//   counts 1+2 and 2+1 separately
// - why not greedy -> {1, 3, 4}, 6: greedy takes 4+1+1 = 3 coins, the best is 3+3 = 2
