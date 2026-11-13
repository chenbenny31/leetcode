// LC 329 Longest Increasing Path in a Matrix (Hard) — grid DFS + memo (implicit DAG)
// Return the number of cells on the longest path that moves up / down / left / right and
// whose values strictly increase.
// Clarify: strictly increasing (equal neighbors do not connect); 4 directions, no
//   diagonals, no wrap-around; non-empty matrix; the answer counts cells, not moves.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Longest increasing path that starts at (r, c).
int dfs(const vector<vector<int>>& matrix, vector<vector<int>>& memo, int r, int c) {
    if (memo[r][c] != 0) { return memo[r][c]; }
    int m = matrix.size();
    int n = matrix[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int best = 1;
    for (int d = 0; d < 4; d++) {
        int nxtR = r + dirs[d][0];
        int nxtC = c + dirs[d][1];
        if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
        // strictly larger only, so the path can never come back: no visited set
        if (matrix[nxtR][nxtC] <= matrix[r][c]) { continue; }
        best = max(best, 1 + dfs(matrix, memo, nxtR, nxtC));
    }
    memo[r][c] = best;
    return best;
}

// Core idea: edges go from a cell to its strictly larger neighbors, which is a DAG, so
// the longest path from a cell is 1 + the best of those neighbors; memoize it and take
// the maximum over all start cells. T: O(m*n), S: O(m*n).
int longestIncreasingPath(const vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    // memo[r][c] = longest increasing path starting at (r, c), 0 = not computed yet
    vector<vector<int>> memo(m, vector<int>(n, 0));
    int longest = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            longest = max(longest, dfs(matrix, memo, r, c));
        }
    }
    return longest;
}

void test(const string& label, const vector<vector<int>>& matrix, int want) {
    cout << label << (longestIncreasingPath(matrix) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("snake", {{1, 2, 3}, {6, 5, 4}}, 6);
    test("peak-in-center", {{1, 2, 1}, {2, 3, 2}, {1, 2, 1}}, 3);
    test("all-equal", {{7, 7}, {7, 7}}, 1);
    test("single-cell", {{5}}, 1);
    return 0;
}

// follow-up (ideas only):
// - why no cycle / no visited set -> every move goes to a strictly larger value, so a
//   path cannot return to a cell; the memo alone stops repeated work
// - topological alternative -> Kahn by out-degree (number of larger neighbors): peel the
//   cells with out-degree 0 layer by layer; the number of layers is the answer
// - water drop flowing to the lowest neighbor (Google) -> same memoized dfs, but it
//   follows one neighbor and returns the destination cell instead of a length
// - longest path in a DAG in general -> the same dfs + memo over the adjacency list, or
//   relax edges in topological order; O(V + E)
// - count the increasing paths (LC 2328) -> memo holds counts: 1 for the cell alone plus
//   the counts of its larger neighbors, summed over all cells mod 1e9 + 7
// - number of paths out of the boundary (LC 576) -> cells can repeat, so the state needs
//   the moves left: memo on (r, c, moves)
