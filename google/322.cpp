// dp bottom-up, T: O(mn), m = coins, n = amount, S: O(n)

#include <vector>
#include <algorithm> // std::min
#include <climits> // INT_MAX

class Solution {
public:
    int coinChange(std::vector<int>& coins, int amount) {
        std::vector<int> dp(amount + 1, INT_MAX);
        dp[0] = 0;

        for (int i = 1; i <= amount; ++i) {
            for (int c : coins) {
                if (c <= i && dp[i-c] != INT_MAX) {
                    dp[i] = std::min(dp[i], dp[i-c] + 1);
                }
            }
        }
        return dp[amount] == INT_MAX ? -1 : dp[amount];
    }
};

// dp[i-c]!=INT_MAX: guard prevents INT_MAX+1 overflows to INT_MIN
// dp[0]=0 base case: zero coins need for amount 0
// inner loop order: coins over amounts, unbounded knapsack allows reuse of coin
// cache sorting helps with inner loop for large amount
// bottom-up over top-down: no recursion, cache friendly, predictable memory
// coins=[2] and amount=3: dp[3]=INT_MAX, returns -1
