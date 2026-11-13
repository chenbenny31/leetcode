// Grid Paths with Checkpoints (Google, interview report). n x m grid (n rows, m columns),
// start (n-1, 0) bottom-left, end (n-1, m-1) bottom-right. Each move goes to the next
// column and to row -1, 0 or +1, staying inside the grid. Counts are mod 1e9+7.
// (1) Count the start -> end paths. n, m >= 1; a 1-column grid has 1 (empty) path.
// (2) Checkpoints (row, col), in any order, must all be visited; two checkpoints in
//     different rows of the same column -> 0. No checkpoints -> same as (1).
//
// Clarify:
// - "row -1, 0 or +1" -> up, straight or down one row per column; never leave the grid
// - counts can be huge -> mod 1e9+7 (stated); long long before each %
// - n == 1 -> one straight path; m == 1 -> start is end, 1 path
// - (2): a checkpoint on the start or end cell? -> fine, always visited; outside the
//   grid -> assume never given
// Idea: column-by-column DP, ways[row] = paths reaching (row, col); a cell sums its three
//   left neighbors, one rolling column. (2): each move advances one column, so at a
//   checkpoint column only the required row keeps its ways (zero the others).
// Pitfalls:
// - take % after each cell sum, not only at the end
// - row - 1 / row + 1 bounds at the top and bottom rows
// - (2): two checkpoints in one column with different rows -> 0 before the DP
// - (2): apply a checkpoint in column 0 too (before any step)
// Tests to write: 3 x 3; one row; one column; a mod wrap; (2): up-then-down checkpoint,
//   unreachable checkpoint, unsorted input, two rows in one column.

#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

const int MOD = 1'000'000'007;

// ways for column col + 1 from ways for column col: each cell sums its three left
// neighbors (row - 1, row, row + 1)
vector<long long> nextColumn(const vector<long long>& ways) {
    int n = ways.size();
    vector<long long> nextWays(n, 0);
    for (int row = 0; row < n; row++) {
        nextWays[row] = ways[row];
        if (row > 0) { nextWays[row] += ways[row - 1]; }
        if (row + 1 < n) { nextWays[row] += ways[row + 1]; }
        nextWays[row] %= MOD;
    }
    return nextWays;
}

// (1) column-by-column DP keeping one rolling column. T: O(n m), S: O(n).
int countPaths(int n, int m) {
    // ways[row] = paths from start to (row, current column)
    vector<long long> ways(n, 0);
    ways[n - 1] = 1;
    for (int col = 1; col < m; col++) { ways = nextColumn(ways); }
    return ways[n - 1];
}

// (2) every move advances one column, so checkpoints are visited in column order and a
// column holds at most one. Same DP; at a checkpoint column zero every other row.
// T: O(n m + c), S: O(n + m).
int countPathsWithCheckpoints(int n, int m, const vector<pair<int, int>>& checkpoints) {
    // required[col] = row that must be visited in column col, -1 if none
    vector<int> required(m, -1);
    for (auto [row, col] : checkpoints) {
        if (required[col] != -1 && required[col] != row) { return 0; }
        required[col] = row;
    }
    vector<long long> ways(n, 0);
    ways[n - 1] = 1;
    for (int col = 0; col < m; col++) {
        if (col > 0) { ways = nextColumn(ways); }
        if (required[col] == -1) { continue; }
        for (int row = 0; row < n; row++) {
            if (row != required[col]) { ways[row] = 0; }
        }
    }
    return ways[n - 1];
}

void test(const string& label, int n, int m, int want) {
    bool ok = countPaths(n, m) == want && countPathsWithCheckpoints(n, m, {}) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testCheckpoints(const string& label, int n, int m,
                     const vector<pair<int, int>>& checkpoints, int want) {
    bool ok = countPathsWithCheckpoints(n, m, checkpoints) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("3x3", 3, 3, 2);
    test("3x4", 3, 4, 4);
    test("one-row", 1, 5, 1);
    test("one-column", 4, 1, 1);
    test("mod-wrap", 2, 32, 73741817);

    testCheckpoints("up-then-down", 3, 3, {{1, 1}}, 1);
    testCheckpoints("unreachable", 3, 3, {{0, 1}}, 0);
    testCheckpoints("unsorted", 3, 5, {{2, 3}, {1, 1}}, 2);
    testCheckpoints("same-column", 3, 4, {{1, 1}, {2, 1}}, 0);
}

// follow-up: blocked cells -> zero ways[row] for blocked cells after each column step
// follow-up: any start row / end row -> seed ways[startRow] = 1, read ways[endRow]
// follow-up: m huge (1e18), n small -> ways evolve by a fixed n x n tridiagonal matrix:
//   matrix power between consecutive checkpoint columns, O(n^3 log m) per segment
// follow-up: visit at least one checkpoint -> total - paths with every checkpoint cell
//   blocked (complement), both from the same DP
