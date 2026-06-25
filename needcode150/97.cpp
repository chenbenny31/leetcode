// 2d-dp, T: O(mn), S: O(mn)

#include <string>
#include <vector>
#include <cstdint> // uint8_t

class Solution {
public:
    bool isInterleave(std::string s1, std::string s2, std::string s3) {
        int m = static_cast<int>(s1.size());
        int n = static_cast<int>(s2.size());
        if (m + n != static_cast<int>(s3.size())) { return false; }

        std::vector<std::vector<uint8_t>> dp(m + 1, std::vector<uint8_t>(n + 1, 0));
        dp[0][0] = 1;

        for (int i = 1; i <= m; i++) {
            dp[i][0] = dp[i-1][0] && (s1[i-1] == s3[i-1]);
        }
        for (int j = 1; j <= n; j++) {
            dp[0][j] = dp[0][j-1] && (s2[j-1] == s3[j-1]);
        }

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                dp[i][j] = (dp[i-1][j] && s1[i-1] == s3[i+j-1]) ||
                           (dp[i][j-1] && s2[j-1] == s3[i+j-1]); 
            }
        }
        return dp[m][n];
    }
};

// 1d-dp, T: O(mn), S: O(n)

#include <string>
#include <vector>
#include <cstdint> // uint8_t

class Solution {
public:
    bool isInterleave(std::string s1, std::string s2, std::string s3) {
        int m = static_cast<int>(s1.size());
        int n = static_cast<int>(s2.size());
        if (m + n != static_cast<int>(s3.size())) { return false; }

        std::vector<uint8_t> dp(n + 1, 0);
        dp[0] = 1;

        for (int j = 1; j <= n; j++) {
            dp[j] = dp[j-1] && (s2[j-1] == s3[j-1]);
        }

        for (int i = 1; i <= m; i++) {
            dp[0] = dp[0] && (s1[i-1] == s3[i-1]);
            for (int j = 1; j <= n; j++) {
                dp[j] = (dp[j] && s1[i-1] == s3[i+j-1]) ||
                        (dp[j-1] && s2[j-1] == s3[i+j-1]);
            }
        }
        return dp[n];
    }
};

// dp[i][j] recurrence: s3[i+j-1] is the current char in s3, either come from s1[i-1] or s2[j-1], OR from both transition
// m + n != s3.size() early exit
// dp[0] updated per row: replace base case
// no prev state need: only need dp[i-1][j] above and dp[i][j-1] left, (diag dp[i-1][j-1] require prev state)
