// 2d-dp, T: O(mn), S: O(mn)

#include <string>
#include <vector>
#include <cstdint> // uint8_t

class Solution {
public:
    bool isMatch(std::string s, std::string p) {
        int m = static_cast<int>(s.size());
        int n = static_cast<int>(p.size());
        std::vector<std::vector<uint8_t>> dp(m + 1, std::vector<uint8_t>(n + 1, 0));
        dp[0][0] = 1;

        // handle patterns like a*, a*b*, matching empty string
        for (int j = 2; j <= n; j += 2) {
            if (p[j-1] == '*') { dp[0][j] = dp[0][j-2]; }
        }

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (p[j-1] == '*') {
                    dp[i][j] = dp[i][j-2]; // zero occurrence of preceding element
                    if (p[j-2] == '.' || p[j-2] == s[i-1]) { // one or more occurrence if preceding matches current
                        dp[i][j] = dp[i][j] || dp[i-1][j];
                    }
                } else if (p[j-1] == '.' || p[j-1] == s[i-1]) {
                    dp[i][j] = dp[i-1][j-1];
                }
            }
        }
        return dp[m][n];
    }
};

// * always pair with preceding element: p[j-1] == '*', p[j-2] is element * quantifies -> zero || one or more 
// dp[0][j]: base case, patterns matching empty string
// dp[i-1][j] not dp[i-1][j-2] for one or more, dp[i-1][j] = already matches one occurrence
