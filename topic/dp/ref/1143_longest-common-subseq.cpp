// LC 1143 Longest Common Subsequence: return the length of the longest common
// subsequence of text1 and text2.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::max;
using std::string;
using std::vector;

// 2D DP over prefixes. T: O(m*n), S: O(m*n).
int longestCommonSubsequence(const string& text1, const string& text2) {
    int m = text1.size();
    int n = text2.size();
    // dp[i][j] = LCS of the first i chars of text1 and the first j chars of text2
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

void test(const string& label, const string& a, const string& b, int want) {
    if (longestCommonSubsequence(a, b) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("interleaved", "axbyc", "abc", 3);
    test("reversed", "xyz", "zyx", 1);
    test("repeat", "aaa", "aa", 2);
    test("empty", "ab", "", 0);
}
