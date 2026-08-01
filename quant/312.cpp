// dp

#include <vector>
#include <algorithm>

class Solution {
public:
    int maxCoins(std::vector<int>& nums) {
        const int n = static_cast<int>(nums.size());
        std::vector<int> balloons(n + 2); // [1, nums, 1]
        balloons[0] = 1;
        balloons[n + 1] = 1;
        for (int i = 0; i < n; i++) {
            balloons[i + 1] = nums[i];
        }

        std::vector<std::vector<int>> dp(n + 2, std::vector<int>(n + 2, 0)); // dp[i][j] = max coins from bursting [i, j]
        for (int len = 1; len <= n; len++) {
            for (int i = 1; i + len - 1 <= n; i++) {
                int j = i + len - 1;
                for (int k = i; k <= j; k++) { // k = last ballon burst in [i, j]
                    int coins = dp[i][k - 1] + dp[k + 1][j]
                              + balloons[i - 1] * balloons[k] * balloons[j + 1]  ;
                    dp[i][j] = std::max(dp[i][j], coins);
                }
            }
        }
        return dp[1][n];
    }
};
