// 112 · Edit Distance — LC 72
// Mother: select/optional/13-dp/05-edit-distance.cpp
// min num of single-char inserts, deletes, replaces to turn word1 into word2
// reported: LC tag #58
//
// clarify:
// operation cost -> 1 each
// empty strings -> allowed
// case -> sensitive
// lens (~500) -> O(m * n) table is fine

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// core idea: DP over prefixes; equal last chars cost nothing, else 1 + best of 3 edits
// T: O(m * n), S: O(m * n)
int minDistance(const string& word1, const string& word2) {
    int m = word1.size();
    int n = word2.size();
    // dp[i][j] = edits to turn word1[0..i) into word2[0..j)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    // to empty: delete all i chars; from empty: insert all j chars
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                // replace, delete word1[i-1], insert word2[j-1]
                dp[i][j] = 1 + min({dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1]});
            }
        }
    }
    return dp[m][n];
}

void test(const string& label, const string& word1, const string& word2, int want) {
    cout << label << (minDistance(word1, word2) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("replace", "ab", "ac", 1);
    test("shift", "abc", "bcd", 2);
    test("classic", "kitten", "sitting", 3);
    test("empty", "", "ab", 2);
}

// follow-up:
// O(n) space -> keep rows prev / cur, cur[0] = i, swap after each row; answer = prev[n]
// only insert / delete allowed -> m + n - 2 * LCS(word1, word2)
