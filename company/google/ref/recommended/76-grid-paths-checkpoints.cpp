// Grid Paths with Checkpoints (Google, interview report)
// n rows x m columns, start (n-1, 0) bottom-left -> end (n-1, m-1) bottom-right
// (1) cnt start -> end paths, mod 1e9+7
// (2) checkpoints (row, col), any order, must all be visited; none -> same as (1)
//
// clarify:
// move set -> one column right, row -1 / 0 / +1, stay inside grid
// n == 1 or m == 1 -> n, m >= 1; 1-column grid has 1 (empty) path
// 2 checkpoints, same column, different rows -> 0
// checkpoint on start / end cell -> fine, treated like any other
// checkpoint outside grid -> assume valid input

#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::pair;

const int MOD = 1'000'000'007;

// ways for next column: each cell sums left neighbors in rows row - 1 .. row + 1
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

// (1) count paths
// core idea: column-by-column DP w/ one rolling column
// T: O(n * m), S: O(n)
int countPaths(int n, int m) {
    // ways[row] = paths from start to (row, current column)
    vector<long long> ways(n, 0);
    ways[n - 1] = 1;
    for (int col = 1; col < m; col++) { ways = nextColumn(ways); }
    return ways[n - 1];
}

// (2) with checkpoints
// core idea: path visits each column once -> at most one checkpoint row per column
// same DP; at a checkpoint column zero every other row
// T: O(n * m + c), S: O(n + m) — c = num checkpoints
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
    test("one-row", 1, 5, 1);
    test("one-column", 4, 1, 1);
    test("mod-wrap", 2, 32, 73741817);

    testCheckpoints("up-then-down", 3, 3, {{1, 1}}, 1);
    testCheckpoints("unreachable", 3, 3, {{0, 1}}, 0);
    testCheckpoints("unsorted", 3, 5, {{2, 3}, {1, 1}}, 2);
    testCheckpoints("same-column", 3, 4, {{1, 1}, {2, 1}}, 0);
}

// follow-up:
// blocked cells -> zero ways[row] for blocked cells after each column step
// any start row / end row -> seed ways[startRow] = 1, read ways[endRow]
// m huge (1e18), n small -> n x n tridiagonal matrix power per segment, O(n^3 log m)
// visit at least one checkpoint -> total - paths w/ every checkpoint cell blocked
