// LC 322 Coin Change (Medium) — unbounded knapsack DP
// unlimited coins per denomination; return fewest coins summing to amount, or -1
//
// clarify:
// coins positive -> dp[sum - coin] is always a smaller sum
// unlimited supply -> unbounded knapsack
// amount 0 -> 0
// unreachable -> -1
// amount small -> O(amount) table fits

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// core idea: dp[sum] = 1 + min over coins of dp[sum - coin], built from sum 1 up
// T: O(amount * coins), S: O(amount)
int coinChange(const vector<int>& coins, int amount) {
    // dp[sum] = fewest coins adding up to sum; amount + 1 = infinity
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

// follow-up:
// cnt combos (LC 518) -> dp[0] = 1, dp[sum] += dp[sum - coin], coins in OUTER loop
// cnt ordered sequences (LC 377) -> same sum, amounts in outer loop (1+2 != 2+1)
// why not greedy -> {1, 3, 4}, 6: greedy 4+1+1 = 3 coins, best 3+3 = 2
