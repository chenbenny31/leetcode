// LC 1143 Longest Common Subsequence (Medium) — 2D DP over two prefixes
// Return the length of the longest subsequence common to text1 and text2 (0 if none).
// Clarify: subsequence, not substring; case-sensitive; an empty string returns 0;
//   length only, or the string itself.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Core idea: compare the last chars of the two prefixes: equal -> diagonal + 1, else
// the better of dropping one char from either side. T: O(m * n), S: O(m * n).
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

// follow-up (ideas only):
// - O(min(m, n)) space -> two rows prev / cur over the shorter string
// - return the LCS string -> backtrack from dp[m][n]: equal chars -> take it, i--, j--;
//   else step toward the larger of dp[i-1][j], dp[i][j-1]; reverse at the end
// - is there a parent of length <= maxLen holding both a and b as subsequences -> the
//   shortest common supersequence has length |a| + |b| - LCS; compare it with maxLen
// - build that shortest supersequence (LC 1092) -> same backtrack, also emitting the
//   chars that are skipped on either side
// - edit distance (LC 72), distinct subsequences (LC 115), interleaving string (LC 97)
//   -> the same 2D table over two prefixes, only the transition differs
// - longest palindromic subsequence (LC 516) -> LCS of s and reverse(s)
