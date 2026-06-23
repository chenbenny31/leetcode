// 2d-dp, T: O(mn), S: O(mn)

#include <string>
#include <vector>
#include <algorithm>

class Solution {
public:
    int longestCommonSubsequence(std::string text1, std::string text2) {
        int m = static_cast<int>(text1.size());
        int n = static_cast<int>(text2.size());
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        for (int i = 1; i <= m ; i++) {
            for (int j = 1; j <= n; j++) {
                if (text1[i-1] == text2[j-1]) {
                    dp[i][j] = dp[i-1][j-1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i-1][j], dp[i][j-1]);
                }
            }
        }
        return dp[m][n];
    }
};

// 1d-dp, T: O(mn), S: O(mn)

#include <string>
#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int longestCommonSubsequence(std::string text1, std::string text2) {
        int m = static_cast<int>(text1.size());
        int n = static_cast<int>(text2.size());
        std::vector<int> dp(n + 1, 0);

        for (int i = 1; i <= m; i++) {
            int diag = 0; // dp[i-1][j-1]
            for (int j = 1; j <= n; j++) {
                int above = dp[j]; // dp[i-1][j]
                if (text1[i-1] == text2[j-1]) { dp[j] = diag + 1; }
                else { dp[j] = std::max(dp[j], dp[j-1]); }
                diag = above; // go right, above beomes diag
            }
        }
        return dp[n];
    }
};

// dp[i][j] recurrence, match: dp[i][j] = dp[i-1][j-1]+1 extend LCS, no match: dp[i][j] = max(dp[i-1][j], dp[i][j-1]) skip one char from either
// 1d compression: diag tracks diagonal dp[i-1][j-1] and carries to next iteration
