// LC 97: return whether s3 is an interleaving of s1 and s2 (all characters of both, each
// string's characters in their own order).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// dp[i][j] = s3[0..i+j) is an interleaving of s1[0..i) and s2[0..j); the last character
// came from s1 or from s2. T: O(mn), S: O(mn).
bool isInterleave(const string& s1, const string& s2, const string& s3) {
    int m = s1.size(), n = s2.size();
    if (m + n != (int)s3.size()) { return false; }
    vector<vector<bool>> dp(m + 1, vector<bool>(n + 1, false));
    dp[0][0] = true;
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i > 0 && dp[i - 1][j] && s1[i - 1] == s3[i + j - 1]) { dp[i][j] = true; }
            if (j > 0 && dp[i][j - 1] && s2[j - 1] == s3[i + j - 1]) { dp[i][j] = true; }
        }
    }
    return dp[m][n];
}

// improved: row i only needs row i - 1, so keep one row. T: O(mn), S: O(n).
bool isInterleaveRow(const string& s1, const string& s2, const string& s3) {
    int m = s1.size(), n = s2.size();
    if (m + n != (int)s3.size()) { return false; }
    // row[j] = dp[i][j] for the current i
    vector<bool> row(n + 1, false);
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 && j == 0) {
                row[j] = true;
                continue;
            }
            bool fromS1 = i > 0 && row[j] && s1[i - 1] == s3[i + j - 1];
            bool fromS2 = j > 0 && row[j - 1] && s2[j - 1] == s3[i + j - 1];
            row[j] = fromS1 || fromS2;
        }
    }
    return row[n];
}

void test(const string& label, const string& s1, const string& s2, const string& s3,
          bool want) {
    bool ok = isInterleave(s1, s2, s3) == want && isInterleaveRow(s1, s2, s3) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("mix", "ab", "cd", "acbd", true);
    test("wrong-order", "ab", "cd", "bacd", false);
    test("shared-char", "aa", "ab", "aaba", true);
    test("length", "a", "b", "abc", false);
    test("empty", "", "", "", true);
}
