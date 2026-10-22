// LC 322 Coin Change: unlimited coins of the given denominations. Return the fewest
// coins that sum to amount, or -1 if impossible.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::min;
using std::string;
using std::vector;

// Unbounded-knapsack DP over amounts. T: O(amount * coins), S: O(amount).
int coinChange(const vector<int>& coins, int amount) {
    // dp[a] = fewest coins summing to a; amount + 1 means unreachable
    vector<int> dp(amount + 1, amount + 1);
    dp[0] = 0;
    for (int a = 1; a <= amount; a++) {
        for (int c : coins) {
            if (c <= a) { dp[a] = min(dp[a], dp[a - c] + 1); }
        }
    }
    return dp[amount] > amount ? -1 : dp[amount];
}

void test(const string& label, const vector<int>& coins, int amount, int want) {
    if (coinChange(coins, amount) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("greedy-fails", {1, 3, 4}, 6, 2);
    test("impossible", {2}, 3, -1);
    test("zero", {5}, 0, 0);
}
