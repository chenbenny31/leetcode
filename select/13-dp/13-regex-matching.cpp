// LC 10 Regular Expression Matching (Hard). Pattern: 2D DP over two prefixes.
// Does pattern p match all of s? '.' matches any one char; '*' matches zero or more
// of the element right before it.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// "x*" either matches nothing or eats one more char of s. T: O(m * n), S: O(m * n).
bool isMatch(const string& s, const string& p) {
    int m = s.size();
    int n = p.size();
    // dp[i][j] = s[0..i) matches p[0..j)
    vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
    dp[0][0] = true;
    // i starts at 0: "a*" can match the empty prefix
    for (int i = 0; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (p[j - 1] == '*') {
                // the x in "x*"
                char starred = p[j - 2];
                // zero copies: drop "x*"
                bool zero = dp[i][j - 2];
                // one more copy: s[i-1] matches x, and s[0..i-1) still matches "..x*"
                bool charMatch = i > 0 && (starred == '.' || starred == s[i - 1]);
                bool more = charMatch && dp[i - 1][j];
                dp[i][j] = zero || more;
            } else {
                char patChar = p[j - 1];
                bool charMatch = i > 0 && (patChar == '.' || patChar == s[i - 1]);
                dp[i][j] = charMatch && dp[i - 1][j - 1];
            }
        }
    }
    return dp[m][n];
}

void test(const string& label, const string& s, const string& p, bool want) {
    cout << label << (isMatch(s, p) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("star-zero", "b", "a*b", true);
    test("star-many", "aaab", "a*b", true);
    test("dot-star", "abc", "a.*", true);
    test("tail-mismatch", "ab", ".*c", false);
}

// follow-up: wildcard (LC 44, '?' one char, '*' any sequence) -> '*': dp[i][j - 1] ||
//   dp[i - 1][j]; '?' like '.', O(m * n)
// follow-up: O(n) space -> row i reads only rows i and i - 1: prev/cur vector<bool>
