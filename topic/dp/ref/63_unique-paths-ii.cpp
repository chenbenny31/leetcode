// LC 63 Unique Paths II: like unique paths, but some cells are obstacles (1) that
// cannot be stepped on. Count right/down paths from top-left to bottom-right.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// Rolling 1D dp; an obstacle zeroes its cell. T: O(m*n), S: O(n).
int uniquePathsWithObstacles(const vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    // dp[j] = paths to column j of the current row; seeded so row 0 starts at 1
    vector<int> dp(n, 0);
    dp[0] = 1;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 1) {
                dp[j] = 0;
            } else if (j > 0) {
                dp[j] += dp[j - 1];
            }
        }
    }
    return dp[n - 1];
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (uniquePathsWithObstacles(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("open", {{0, 0}, {0, 0}}, 2);
    test("centre-block", {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}, 2);
    test("start-blocked", {{1, 0}, {0, 0}}, 0);
    test("end-blocked", {{0, 0}, {0, 1}}, 0);
}
