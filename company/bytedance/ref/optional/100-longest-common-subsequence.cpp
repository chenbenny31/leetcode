// ByteDance / LC 1143: length of the longest common subsequence of text1 and text2.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// 2D dp over prefixes. T: O(m*n), S: O(m*n).
int longestCommonSubsequence(const string& text1, const string& text2) {
    int m = text1.size();
    int n = text2.size();
    // dp[i][j] = LCS length of text1[0..i) and text2[0..j)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (text1[i - 1] == text2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                // drop the last char of one string or the other
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    return dp[m][n];
}

void test(const string& label, const string& text1, const string& text2, int want) {
    bool ok = longestCommonSubsequence(text1, text2) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", "abcde", "ace", 3);
    test("same", "abc", "abc", 3);
    test("disjoint", "abc", "def", 0);
}

// follow-up: O(min(m, n)) space -> two rows prev/cur over the shorter string, O(m*n) time
// follow-up: return the LCS string -> backtrack from dp[m][n]: chars equal -> take, i--,
//   j--; else step toward the larger of dp[i-1][j], dp[i][j-1]
// follow-up: longest common substring -> dp[i][j] = match ? dp[i-1][j-1] + 1 : 0;
//   answer = max dp[i][j], O(m*n)
// follow-up: edit distance (LC 72) -> dp[i][j] = match ? dp[i-1][j-1] : 1 + min(ins, del,
//   replace), O(m*n)
