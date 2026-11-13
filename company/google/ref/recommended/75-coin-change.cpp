// 75 · Coin Change — LC 322
// Mother: select/13-dp/02-coin-change.cpp
// unlimited coins of each denomination; fewest coins that sum to amount, -1 if impossible
//
// clarify:
// amount 0 -> 0 coins
// no combination -> -1
// coins positive -> yes
// how large is amount -> dp table is O(amount)

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// core idea: unbounded knapsack; dp[sum] = 1 + min over coins of dp[sum - coin]
// T: O(amount * coins), S: O(amount)
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

// follow-up:
// why not greedy (largest coin first) -> {1, 3, 4}, 6: greedy takes 4+1+1, best is 3+3
// cnt combinations (LC 518) -> dp[sum] += dp[sum - coin], dp[0] = 1, coins in outer loop
