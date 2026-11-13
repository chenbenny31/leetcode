// 69 · Coin Change — LC 322
// Mother problem: select/13-dp/02-coin-change.cpp
//
// LC 322 (Medium), unbounded knapsack DP: unlimited coins of each denomination; return
// the fewest coins that sum to amount, or -1 if no combination works.
//
// Clarify
// - no combination works → -1; amount 0 → 0 coins
// - coins positive, amount up to ~1e4 → O(amount * coins) DP is fine
//
// Core idea
// - dp[sum] = 1 + min over coins of dp[sum - coin], dp[0] = 0; amount + 1 is infinity.
// - Greedy fails: {1, 3, 4}, 6 → greedy 4+1+1, best 3+3.
// - T O(amount * coins), S O(amount).
//
// Tests: greedy fails, mixed, impossible, amount 0
//
// Follow-ups
// - count combinations (LC 518) → dp[sum] += dp[sum - coin], dp[0] = 1, coins in the
//   outer loop so each multiset counts once (amounts outer counts every ordering)
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// dp[sum] = 1 + min over coins of dp[sum - coin]. T: O(amount * coins), S: O(amount).
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
