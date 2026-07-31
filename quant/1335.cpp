// dp

#include <vector>
#include <algorithm> // std::max, std::min
#include <climits> // INT_MAX

class Solution {
public:
    int minDifficulty(std::vector<int>& jobDifficulty, int d) {
        const int n = static_cast<int>(jobDifficulty.size());
        if (n < d) { return -1; }

        std::vector<std::vector<int>> dp(n + 1, std::vector<int>(d + 1, INT_MAX / 2)); // dp[i][k] = min total diff cover i jobs over k days
        dp[0][0] = 0;

        for (int i = 1; i <= n; i++) {
            for (int k = 1; k <= std::min(i, d); k++) {
                int curMax = 0;
                for (int p = i - 1; p >= k - 1; p--) { // final day and first k-1 days need >1job, run backwards to prevent re-compute max
                    curMax = std::max(curMax, jobDifficulty[p]);
                    dp[i][k] = std::min(dp[i][k], dp[p][k - 1] + curMax);
                }
            }
        }
        return dp[n][d];
    }
};
