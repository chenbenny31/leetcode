// dp-2d, T: O(mn), S: O(mn)

#include <vector>

class Solution {
public:
    int uniquePaths(int m, int n) {
        std::vector<std::vector<int>> dp(m, std::vector<int>(n, 1)); // dp[r][c] = num of ways to reach (r, c), from left or above

        for (int r = 1; r < m; ++r) {
            for (int c = 1; c < n; ++c) {
                dp[r][c] = dp[r-1][c] + dp[r][c-1];
            }
        }
        return dp[m-1][n-1];
    }
};

// dp-1d, T: O(mn), S: O(n)

#include <vector>

class Solution {
public:
    int uniquePaths(int m, int n) {
        std::vector<int> dp(n, 1); // dp[c] = num of ways to reach from above or left in current row

        for (int r = 1; r < m; ++r) {
            for (int c = 1; c < n; ++c) {
                dp[c] += dp[c-1];
            }
        }
        return dp[n-1];
    }
};

// cache: dp-2d may evict previous row from L1, dp-1d is always in L1
