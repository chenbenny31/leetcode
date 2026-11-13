// LC 994 Rotting Oranges (Medium) — multi-source BFS by layers
// grid of 0 (empty), 1 (fresh), 2 (rotten); each minute fresh oranges 4-adjacent to a
// rotten one rot. Return the minutes until no fresh orange remains, or -1 if impossible.
// Clarify: no fresh orange at the start returns 0; a fresh orange that never rots
//   returns -1; 4 directions; may I modify the grid.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;

// Core idea: push every rotten orange first so the BFS starts from all of them, one layer
// per minute; count fresh up front and decrement per rot, so fresh > 0 at the end is -1.
// T: O(m*n), S: O(m*n).
int orangesRotting(vector<vector<int>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // oranges that rotted in the latest minute
    queue<pair<int, int>> q;
    int fresh = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 2) { q.push({r, c}); }
            if (grid[r][c] == 1) { fresh++; }
        }
    }
    int minutes = 0;
    // stop once fresh hits 0, so the last layer does not count an extra minute
    while (!q.empty() && fresh > 0) {
        minutes++;
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            auto [curR, curC] = q.front();
            q.pop();
            for (int d = 0; d < 4; d++) {
                int nxtR = curR + dirs[d][0];
                int nxtC = curC + dirs[d][1];
                if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
                if (grid[nxtR][nxtC] != 1) { continue; }
                grid[nxtR][nxtC] = 2;
                fresh--;
                q.push({nxtR, nxtC});
            }
        }
    }
    return fresh == 0 ? minutes : -1;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (orangesRotting(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("spread", {{2, 1, 1}, {0, 1, 0}}, 2);
    test("two-sources", {{2, 1, 1, 1, 2}}, 2);
    test("unreachable", {{2, 0, 1}}, -1);
    test("no-fresh", {{0, 2}}, 0);
    return 0;
}

// follow-up (ideas only):
// - distance from every cell to the nearest 0 (LC 542, Google "nearest taxi") -> same
//   multi-source BFS without layers: dist is set on push, -1 = not reached yet
// - walls and gates (LC 286) -> multi-source BFS from every gate, in place; step only
//   into INT_MAX rooms, so walls and gates are never overwritten
// - shortest clear path, 8 directions (LC 1091) -> single-source BFS with dist counting
//   cells; check the start and end cells first (1 x 1 open grid -> 1)
// - per-cell rot delays -> multi-source Dijkstra on time: heap of (time, r, c) with all
//   rotten cells at 0; answer = max time over fresh cells, -1 if one is never reached
// - as far from land as possible (LC 1162) -> multi-source BFS from all land; the last
//   layer's distance is the answer
