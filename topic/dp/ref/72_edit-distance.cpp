// LC 72 Edit Distance: minimum insert / delete / replace operations to turn word1 into
// word2.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::min;
using std::string;
using std::vector;

// 2D DP over prefixes: equal last chars cost nothing, else 1 + the cheapest of
// replace (diagonal), delete (up) and insert (left). T: O(m*n), S: O(m*n).
int minDistance(const string& word1, const string& word2) {
    int m = word1.size(), n = word2.size();
    // dp[i][j] = edits to turn word1[0..i) into word2[0..j)
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
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
                dp[i][j] = 1 + min({dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1]});
            }
        }
    }

    return dp[m][n];
}

void test(const string& label, const string& a, const string& b, int want) {
    cout << label << (minDistance(a, b) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("replace", "cat", "cut", 1);
    test("swap", "ab", "ba", 2);
    test("insert-all", "", "abc", 3);
    test("delete-all", "abc", "", 3);
}
