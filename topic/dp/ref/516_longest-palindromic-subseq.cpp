// LC 516 Longest Palindromic Subsequence: return the length of the longest palindromic
// subsequence of s.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::max;
using std::string;
using std::vector;

// Interval DP on s[i..j], filled with i descending so shorter intervals are ready.
// T: O(n^2), S: O(n^2).
int longestPalindromeSubseq(const string& s) {
    int n = s.size();
    // dp[i][j] = longest palindromic subsequence inside s[i..j]
    vector<vector<int>> dp(n, vector<int>(n, 0));
    for (int i = n - 1; i >= 0; i--) {
        dp[i][i] = 1;
        for (int j = i + 1; j < n; j++) {
            if (s[i] == s[j]) {
                dp[i][j] = dp[i + 1][j - 1] + 2;
            } else {
                dp[i][j] = max(dp[i + 1][j], dp[i][j - 1]);
            }
        }
    }
    return n == 0 ? 0 : dp[0][n - 1];
}

void test(const string& label, const string& s, int want) {
    if (longestPalindromeSubseq(s) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("skip-inside", "agbdba", 5);
    test("distinct", "abc", 1);
    test("pair", "aa", 2);
    test("empty", "", 0);
}
