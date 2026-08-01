// dp

class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        const int m = static_cast<int>(text1.size());
        const int n = static_cast<int>(text2.size());

        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1, 0)); // dp[i][j] = LCS len of text1[0..i) and text2[0..j)

        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (text1[i - 1] == text2[j - 1]) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[m][n];
    }
};

// 1d-dp

class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        if (text1.length() < text2.length()) { std::swap(text1, text2); }
        const int m = static_cast<int>(text1.size());
        const int n = static_cast<int>(text2.size());

        std::vector<int> dp(n + 1, 0);
        for (int i = 1; i <= m; i++) {
            int prevDiag = 0; // holds dp[i-1][j-1]
            for (int j = 1; j <= n; j++) {
                int tmp = dp[j]; // dp[i-1][j]
                if (text1[i - 1] == text2[j - 1]) {
                    dp[j] = prevDiag + 1;
                } else {
                    dp[j] = std::max(dp[j], dp[j - 1]);
                }
                prevDiag = tmp;
            }
        }
        return dp[n];
    }
};
