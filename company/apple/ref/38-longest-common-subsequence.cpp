// return the len of the longest common subsequence of text1 and text2

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;
using std::swap;

// 2d-dp over prefixes, T: O(mn), S: O(mn)
int longestCommonSubsequence(const string& text1, const string& text2) {
    int m = text1.size();
    int n = text2.size();
    // dp[i][j] = LCS len of text1[0..i) and text2[0..j)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                // drop the last char of one side
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

// row i only reads row i-1, keep two rows, T: O(mn), S: O(n)
int longestCommonSubsequenceRows(const string& text1, const string& text2) {
    int m = text1.size();
    int n = text2.size();
    // prev[j] = dp[i-1][j], cur[j] = dp[i][j]
    vector<int> prev(n + 1, 0), cur(n + 1, 0);
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                cur[j] = prev[j - 1] + 1;
            } else {
                cur[j] = max(prev[j], cur[j - 1]);
            }
        }
        swap(prev, cur);
    }
    return prev[n];
}

void test(const string& label, const string& text1, const string& text2, int want) {
    bool ok = longestCommonSubsequence(text1, text2) == want &&
              longestCommonSubsequenceRows(text1, text2) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("gap", "abc", "ac", 2);
    test("cross", "ab", "ba", 1);
    test("none", "ab", "cd", 0);
    test("empty", "", "ab", 0);
}
