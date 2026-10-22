// LC 10 Regular Expression Matching: does pattern p match the whole of s?
// '.' matches any single char; 'x*' matches zero or more of the char x.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// 2D dp over (prefix of s, prefix of p). T: O(m*n), S: O(m*n).
bool isMatch(const string& s, const string& p) {
    int m = s.size(), n = p.size();
    // dp[i][j] = 1 if s[0..i) fully matches p[0..j)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    dp[0][0] = 1;
    for (int i = 0; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (p[j - 1] == '*') {
                // zero copies of p[j-2], or one more copy of it on top of dp[i-1][j]
                bool more = i > 0 && (p[j - 2] == '.' || p[j - 2] == s[i - 1]);
                dp[i][j] = dp[i][j - 2] || (more && dp[i - 1][j]);
            } else {
                bool same = i > 0 && (p[j - 1] == '.' || p[j - 1] == s[i - 1]);
                dp[i][j] = same && dp[i - 1][j - 1];
            }
        }
    }
    return dp[m][n];
}

void test(const string& label, const string& s, const string& p, bool want) {
    cout << label << (isMatch(s, p) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("dot", "abc", "a.c", true);
    test("star", "aaa", "a*", true);
    test("fail", "ab", ".*c", false);
    test("empty", "", "a*b*", true);
}
