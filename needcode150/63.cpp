// 1d-dp, T: O(mn), S: O(n)

#include <vector>

class Solution {
public:
    int uniquePathsWithObstacles(std::vector<std::vector<int>>& obstacleGrid) {
        int m = static_cast<int>(obstacleGrid.size());
        int n = static_cast<int>(obstacleGrid[0].size());

        if (obstacleGrid[0][0] == 1 || obstacleGrid[m-1][n-1] == 1) { return 0; }
        std::vector<long long> dp(n, 0);
        dp[0] = 1;

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (obstacleGrid[r][c] == 1) {
                    dp[c] = 0;
                } else if (c > 0) {
                    dp[c] += dp[c-1];
                }
            }
        }
        return static_cast<int>(dp[n-1]);
    }
};

// obstacle handling: zero out dp[c], no path through this cell
// long long for dp: path count could exceed INT_MAX
