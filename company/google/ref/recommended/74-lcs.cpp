// 74 · Longest Common Subsequence — LC 1143
// Mother problem: select/13-dp/04-longest-common-subsequence.cpp
//
// LC 1143 (Medium), 2D DP over two prefixes: return the length of the longest
// subsequence common to text1 and text2 (0 if none).
// Google reports: common parent sequence
//
// Clarify
// - subsequence, not substring → gaps allowed; empty string → 0
// - lengths up to ~1000 → O(m * n) DP
//
// Core idea
// - dp[i][j] = LCS of text1[0..i) and text2[0..j): equal last chars → dp[i-1][j-1] + 1,
//   else max(dp[i-1][j], dp[i][j-1]). T O(m * n), S O(m * n).
//
// Tests: skipped chars, crossing order, disjoint, empty
//
// Follow-ups
// - shortest common parent of a and b within maxLen → |a| + |b| - LCS <= maxLen
// - O(min(m, n)) space → two rows prev / cur over the shorter string
// - return the LCS string → backtrack from dp[m][n]: equal chars → take, i--, j--;
//   else step toward the larger of dp[i-1][j], dp[i][j-1]; reverse at the end
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Match last chars -> diagonal + 1, else drop one char from either side.
// T: O(m * n), S: O(m * n).
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
