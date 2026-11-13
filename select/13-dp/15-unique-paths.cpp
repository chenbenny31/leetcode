// LC 62 Unique Paths (Medium) — grid DP
// cnt paths from top-left to bottom-right of an m x n grid; moves = right or down
//
// clarify:
// allowed moves -> right / down only
// grid size, cnt fits in int? -> use long long
// obstacles -> none; else follow-up (LC 63)
// 1 x 1 grid -> 1 path

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// core idea: paths to a cell = paths from above + paths from left; one rolling row
// T: O(m * n), S: O(n)
long long uniquePaths(int m, int n) {
    // dp[col] = paths to (row, col); first row and first column are all 1
    vector<long long> dp(n, 1);
    for (int row = 1; row < m; row++) {
        for (int col = 1; col < n; col++) {
            // old dp[col] = cell above, dp[col - 1] = cell on the left
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

// follow-up:
// obstacles (LC 63) -> dp = 0 on a blocked cell (first row / column stop at a block)
// min path sum (LC 64) -> dp = cell + min(top, left)
// moves = one column right w/ row -1 / 0 / +1 (Google) -> column by column, fresh array
// must pass checkpoints (in order: columns must rise) -> multiply cnts of the segments
// maximal square (LC 221) -> dp = 1 + min(top, left, top-left) on a '1' cell
// closed form -> C(m + n - 2, m - 1); multiply and divide step by step in long long
