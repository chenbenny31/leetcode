// LC 64 Minimum Path Sum: given a grid of non-negative numbers, find the path from
// top-left to bottom-right (moving right or down) with the smallest sum.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using std::cout;
using std::min;
using std::string;
using std::vector;

// Rolling 1D dp: best = cell + min(from above, from left). T: O(m*n), S: O(n).
int minPathSum(const vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    // dp[j] = cheapest cost to reach column j of the current row
    vector<int> dp(n, INT_MAX);
    dp[0] = 0;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (j > 0) {
                dp[j] = min(dp[j], dp[j - 1]);
            }
            dp[j] += grid[i][j];
        }
    }
    return dp[n - 1];
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (minPathSum(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {{5}}, 5);
    test("row", {{1, 2, 3}}, 6);
    test("detour", {{1, 9, 1}, {1, 9, 1}, {1, 1, 1}}, 5);
    test("square", {{1, 2}, {1, 1}}, 3);
}
