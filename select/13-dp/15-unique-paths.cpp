// LC 62 Unique Paths (Medium) — grid DP
// Count the paths from the top-left to the bottom-right cell of an m x n grid when each
// move goes one cell right or one cell down.
// Clarify: allowed moves; grid size and whether the count fits in int; obstacles; a
//   1 x 1 grid counts as one path.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: paths to a cell = paths to the cell above + paths to the cell on its left;
// one row is enough because the row above is still in dp when it is updated.
// T: O(m * n), S: O(n).
long long uniquePaths(int m, int n) {
    // dp[col] = paths to (row, col); the first row and first column are all 1
    vector<long long> dp(n, 1);
    for (int row = 1; row < m; row++) {
        for (int col = 1; col < n; col++) {
            // old dp[col] is the cell above, dp[col - 1] is the cell on the left
            dp[col] += dp[col - 1];
        }
    }
    return dp[n - 1];
}

void test(const string& label, int m, int n, long long want) {
    cout << label << (uniquePaths(m, n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single-cell", 1, 1, 1);
    test("single-row", 1, 5, 1);
    test("two-by-two", 2, 2, 2);
    test("three-by-four", 3, 4, 10);
    return 0;
}

// follow-up (ideas only):
// - obstacles (LC 63) -> dp = 0 on a blocked cell (first row / column stop at a block)
// - min path sum (LC 64) -> dp = cell + min(top, left) instead of the sum
// - each move goes one column right with row -1 / 0 / +1 (Google) -> go column by
//   column: new[row] = old[row - 1] + old[row] + old[row + 1], with a fresh array
// - must pass given checkpoints -> multiply the counts of the segments between them, or
//   in the column DP set every other row to 0 at a checkpoint's column
// - maximal square (LC 221) -> dp = 1 + min(top, left, top-left) on a '1' cell
// - closed form -> C(m + n - 2, m - 1): choose which of the moves go down; multiply and
//   divide step by step in long long
