// 2d-dp, T: O(mn), S: O(mn)

#include <vector>

class Solution {
public:
    int uniquePaths(int m, int n) {
        std::vector<std::vector<int>> dp(m, std::vector<int>(n, 1));

        for (int r = 1; r < m; r++) {
            for (int c = 1; c < n; c++) {
                dp[r][c] = dp[r-1][c] + dp[r][c-1];
            }
        }
        return dp[m-1][n-1];
    }
};

// 1d-dp, T: O(mn), S: O(mn)

#include <vector>

class Solution {
public:
    int uniquePaths(int m, int n) {
        std::vector<int> dp(n, 1); // dp[i] = num of ways to reach i-th col at curr row

        for (int r = 1; r < m; r++) {
            for (int c = 1; c < n; c++) {
                dp[c] += dp[c-1];
            }
        }
        return dp[n-1];
    }
};

// 1d-dp: in-place row compression, dp[c] already contain info from last row
