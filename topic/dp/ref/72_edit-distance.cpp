// LC 72 Edit Distance: min number of insert / delete / replace operations to turn
// word1 into word2.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::min;
using std::string;
using std::vector;

// Prefix dp compressed to one row with a saved diagonal. T: O(m*n), S: O(n).
int minDistance(const string& word1, const string& word2) {
    int m = word1.size(), n = word2.size();
    // dp[j] = edits to turn word1[0..i) into word2[0..j) for the current row i
    vector<int> dp(n + 1, 0);
    for (int j = 0; j <= n; j++) {
        dp[j] = j;
    }
    for (int i = 1; i <= m; i++) {
        // diag = dp[i-1][j-1], saved before the in-place overwrite
        int diag = dp[0];
        dp[0] = i;
        for (int j = 1; j <= n; j++) {
            int up = dp[j];
            if (word1[i - 1] == word2[j - 1]) {
                dp[j] = diag;
            } else {
                dp[j] = 1 + min({diag, up, dp[j - 1]});
            }
            diag = up;
        }
    }
    return dp[n];
}

void test(const string& label, const string& a, const string& b, int want) {
    cout << label << (minDistance(a, b) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("replace", "cat", "cut", 1);
    test("swap", "ab", "ba", 2);
    test("insert-all", "", "abc", 3);
    test("delete-all", "abc", "", 3);
    test("same", "abc", "abc", 0);
}
