// LC 62 Unique Paths: an m x n grid, a robot starts at the top-left and moves only
// right or down. How many paths reach the bottom-right?
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Rolling 1D dp: a cell = paths from above + paths from left. T: O(m*n), S: O(n).
int uniquePaths(int m, int n) {
    // dp[j] = paths to column j of the current row (first row is all 1)
    vector<int> dp(n, 1);
    for (int i = 1; i < m; i++) {
        for (int j = 1; j < n; j++) {
            dp[j] += dp[j - 1];
        }
    }
    return dp[n - 1];
}

void test(const string& label, int m, int n, int want) {
    cout << label << (uniquePaths(m, n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("square", 3, 3, 6);
    test("single-row", 1, 5, 1);
    test("single-cell", 1, 1, 1);
    test("tall", 4, 2, 4);
}
