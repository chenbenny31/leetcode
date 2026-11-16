// Longest Water-Flow Path (Google, interview report)
// m x n heights; water moves between 4-neighbors, cur -> nxt
// allowed if nxt < cur (downhill)
// allowed if nxt > cur and the cell before cur is higher than nxt (momentum)
// return the max num of cells on one path; any start cell
// reported: x3 (L3 early-career onsite, L4 onsite, L5 onsite)
//
// clarify:
// equal heights -> report says <= / >=, which can loop forever; assumed strict, ask
// first move -> no cell before it, so downhill only
// len -> cells on the path, 1 for a lone cell
// a cell twice on one path -> allowed if it is entered from another side
// diagonals -> no

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// longest path from (r, c) when it was entered by move dirs[from]; from = 4 -> path start
int dfs(const vector<vector<int>>& grid, int r, int c, int from,
        vector<vector<vector<int>>>& memo) {
    if (memo[r][c][from] != 0) { return memo[r][c][from]; }
    int m = grid.size();
    int n = grid[0].size();
    int result = 1;
    for (int d = 0; d < 4; d++) {
        int nr = r + dirs[d][0];
        int nc = c + dirs[d][1];
        if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
        bool down = grid[nr][nc] < grid[r][c];
        bool pushed = false;
        if (from != 4 && grid[nr][nc] > grid[r][c]) {
            // height of the cell before (r, c)
            int before = grid[r - dirs[from][0]][c - dirs[from][1]];
            pushed = before > grid[nr][nc];
        }
        if (down || pushed) { result = max(result, 1 + dfs(grid, nr, nc, d, memo)); }
    }
    memo[r][c][from] = result;
    return result;
}

// core idea: what may follow depends on the cell before -> state = (cell, entry move)
// an uphill step lands below the cell 2 steps back -> no state repeats -> DFS + memo
// T: O(m * n), S: O(m * n) — 5 states per cell, 4 moves each
int longestFlow(const vector<vector<int>>& grid) {
    if (grid.empty() || grid[0].empty()) { return 0; }
    int m = grid.size();
    int n = grid[0].size();
    // memo[r][c][from] = longest path from (r, c) entered by move from; 0 = not computed
    vector<vector<vector<int>>> memo(m, vector<vector<int>>(n, vector<int>(5, 0)));
    int result = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) { result = max(result, dfs(grid, r, c, 4, memo)); }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (longestFlow(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("downhill-only", {{3, 2, 1}}, 3);
    test("climb-then-fall-back", {{5, 1, 4}}, 4);
    test("equal-peak-blocks", {{3, 1, 3}}, 2);
    test("re-enter-valley", {{5, 1, 4}, {9, 9, 3}}, 6);
    test("single-cell", {{7}}, 1);
    test("empty", {}, 0);
}

// follow-up:
// why memo[r][c] alone is wrong -> the same cell allows more moves after a higher cell
// rule w/ <= and >= (as posted) -> flat cells form cycles; ask for strict or "no revisit"
// "no cell twice" rule -> longest simple path, exponential in general; say so
// return the path -> store the best next move per state, walk it from the best start
// only downhill moves (LC 329) -> memo[r][c] is enough
// momentum lasts k steps -> add steps-left to the state
// very large grid -> recursion depth up to m * n; explicit stack or sort cells by height
