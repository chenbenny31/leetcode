// 2d-dp, T: O(mn), S: O(mn)

#include <string>
#include <vector>
#include <algorithm>

class Solution {
public:
    int minDistance(std::string word1, std::string word2) {
        int m = static_cast<int>(word1.size());
        int n = static_cast<int>(word2.size());
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0));

        for (int i = 0; i <= m; i++) { dp[i][0] = i; }
        for (int j = 0; j <= n; j++) { dp[0][j] = j; }

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (word1[i-1] == word2[j-1]) {
                    dp[i][j] = dp[i-1][j-1];
                } else {
                    dp[i][j] = 1 + std::min({dp[i-1][j-1], // replace
                                             dp[i-1][j], // delete
                                             dp[i][j-1]}); // insert
                }
            }
        }
        return dp[m][n];
    }
};

// 1d-dp, T: O(mn), S: O(mn)

#include <string>
#include <vector>
#include <algorithm>

class Solution {
public:
    int minDistance(std::string word1, std::string word2) {
        int m = static_cast<int>(word1.size());
        int n = static_cast<int>(word2.size());
        std::vector<int> dp(n + 1);

        for (int j = 0; j <= n; j++) { dp[j] = j; }

        for (int i = 1; i <= m; i++) {
            int diag = dp[0]; // dp[i-1][j-1]
            dp[0] = i; // dp[i][0] = i deletions
            for (int j = 1; j <= n; j++) {
                int above = dp[j];
                if (word1[i-1] == word2[j-1]) { dp[j] = diag; }
                else { dp[j] = 1 + std::min({diag, dp[j], dp[j-1]}); }
                diag = above;
            }
        }
        return dp[n];
    }
};

// three operations -> three transition: replace for word[i-1] to word2[j-1]; deletion for word1[i-1]; insertion for word2[j-1]
// base case: dp[i][0] = i: delete i chars of word1, dp[0][j] = j, insert all j chars of word2
