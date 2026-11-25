// 80 · Longest Common Subsequence — LC 1143
// Mother: select/13-dp/04-longest-common-subsequence.cpp
// return len of longest subsequence common to text1 and text2 (0 if none)
// reported: common parent sequence
//
// clarify:
// subsequence vs substring -> subsequence, gaps allowed
// empty string -> 0
// lengths -> ~1000, O(m * n) ok
// need the string itself? -> follow-up

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// core idea: 2D DP over two prefixes; equal last chars -> diagonal + 1
// else max of dropping last char of either prefix
// T: O(m * n), S: O(m * n)
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
    test("skip-chars", "axbycz", "abc", 3);
    test("crossing", "abcd", "acbd", 3);
    test("disjoint", "abc", "def", 0);
    test("empty", "", "abc", 0);
}

// follow-up:
// shortest common parent of a, b within maxLen (Google) -> |a| + |b| - LCS <= maxLen
// O(min(m, n)) space -> two rows prev / cur over the shorter string
// return the LCS string -> backtrack from dp[m][n]: equal chars -> take, i--, j--
// backtrack, chars differ -> step toward larger of dp[i-1][j], dp[i][j-1]; reverse at end
