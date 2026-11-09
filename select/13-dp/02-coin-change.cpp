// LC 322 Coin Change (Medium). Pattern: unbounded knapsack DP.
// Unlimited coins of each denomination; return the fewest coins that sum to amount,
// or -1 if no combination works.

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

// follow-up: number of combinations (LC 518) -> dp[sum] += dp[sum - coin], coins in the
//   OUTER loop, so each multiset is counted once (amounts outer counts orderings)
// follow-up: why not greedy -> {1, 3, 4}, 6: greedy 4+1+1 = 3 coins, best 3+3 = 2
