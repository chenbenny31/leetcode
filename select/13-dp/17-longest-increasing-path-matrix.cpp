// LC 329 Longest Increasing Path in a Matrix (Hard) — grid DFS + memo (implicit DAG)
// return num of cells on the longest strictly increasing path; moves = 4 directions
//
// clarify:
// strictly increasing -> equal neighbors do not connect
// 4 directions, no diagonals, no wrap-around -> bounds check only
// non-empty matrix -> matrix[0] is safe
// answer counts cells, not moves -> single cell = 1

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// longest increasing path that starts at (r, c)
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
        // strictly larger only, so path never comes back: no visited set
        if (matrix[nxtR][nxtC] <= matrix[r][c]) { continue; }
        best = max(best, 1 + dfs(matrix, memo, nxtR, nxtC));
    }
    memo[r][c] = best;
    return best;
}

// core idea: edges to strictly larger neighbors = DAG; path from cell = 1 + best neighbor
// memoize per cell; answer = max over all start cells
// T: O(m * n), S: O(m * n)
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

// follow-up:
// why no visited set -> every move goes strictly larger, path cannot return
// topological alternative -> Kahn by out-degree, peel layer by layer; layers = answer
// water drop to the lowest neighbor (Google) -> same memo dfs, returns destination cell
// longest path in a general DAG -> same dfs + memo over adjacency list, O(V + E)
// cnt increasing paths (LC 2328) -> memo holds cnts: 1 + cnts of larger neighbors
// paths out of the boundary (LC 576) -> cells repeat, memo on (r, c, moves left)
