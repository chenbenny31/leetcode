// LC 72 Edit Distance (Medium). Pattern: 2D DP over two prefixes.
// Return the min number of single-char inserts, deletes and replaces that turn word1
// into word2.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

// Equal last chars cost nothing; else 1 + best of replace / delete / insert.
// T: O(m * n), S: O(m * n).
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

// follow-up: O(n) space -> row i reads only row i-1: prev/cur rows, cur[0] = i,
//   swap(prev, cur) after each row; answer prev[n]
// follow-up: only insert / delete allowed -> m + n - 2 * LCS(word1, word2)
