// 2d-dp, T: O(mn), S: O(mn)

#include <string>
#include <vector>

class Solution {
public:
    int numDistinct(std::string s, std::string t) {
        int m = static_cast<int>(s.size());
        int n = static_cast<int>(t.size());
        std::vector<std::vector<unsigned int>> dp(m + 1, std::vector<unsigned int>(n + 1, 0));

        for (int i = 0; i <= m; i++) { dp[i][0] = 1; } // empty t matched by any prefix

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                dp[i][j] = dp[i-1][j]; // skip s[i-1]
                if (s[i-1] == t[j-1]) { dp[i][j] += dp[i-1][j-1]; } // match s[i-1] with t[j-1]
            }
        }
        return static_cast<int>(dp[m][n]);
    }
};

// 1d-dp, T: O(mn), S: O(n)

#include <string>
#include <vector>

class Solution {
public:
    int numDistinct(std::string s, std::string t) {
        int m = static_cast<int>(s.size());
        int n = static_cast<int>(t.size());
        std::vector<unsigned int> dp(n + 1, 0);
        dp[0] = 1; // empty t, one way

        for (int i = 1; i <= m; i++) {
            for (int j = n; j >= 1; j--) { // reverse - 0/1 knapsack
                if (s[i-1] == t[j-1]) {
                    dp[j] += dp[j-1]; // dp[i-1][j-1] = prev diag
                }
            }
        }
        return static_cast<int>(dp[n]);
    }
};

// dp[i][j] recurrence: inherit dp[i-1][j], can skip s[i-1], increment with dp[i-1][j-1] with s[i-1] matching t[j-1]
// dp[i][0] = 1: base case, empty t matched by any prefix s - exact one way
// 1d reverse scan: 0/1 knapsack, reverse ensure dp[j-1] not yet updated when dp[j] computed
