// LC 994 Rotting Oranges (Medium). Pattern: multi-source BFS by layers.
// grid of 0 (empty), 1 (fresh), 2 (rotten); each minute fresh oranges 4-adjacent to a
// rotten one rot. Return minutes until no fresh orange remains, or -1 if impossible.
//
// Clarify:
// - no fresh orange at the start -> 0 (even with no rotten one)
// - a fresh orange that can never rot -> -1
// - neighbors: 4 directions
// - may I modify the grid? -> here the function takes a copy and rots cells to 2
// - size up to ~1e4 cells -> one BFS
// Idea: push every rotten orange before the loop, so the BFS starts from all of them;
//   one layer = one minute. Count fresh first and decrement on each rot: fresh > 0 at
//   the end means -1, with no second scan.
// Pitfalls:
// - save q.size() before the inner loop; the queue grows while you pop
// - loop while fresh > 0, or the last layer (which rots nothing) adds a minute
// - mark rotten on push, as in islands
// Tests to write: rot spreads around an empty cell; two sources meet in the middle;
//   fresh walled off -> -1; no fresh -> 0.

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

// BFS from all rotten oranges at once, one layer per minute. T: O(m*n), S: O(m*n).
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

// follow-up (LC 542, Google "nearest taxi"): distance from every cell to the nearest 0.
// Same multi-source BFS, but no layers: dist is set on push. T: O(m*n), S: O(m*n).
vector<vector<int>> nearestZeroDistance(const vector<vector<int>>& mat) {
    int m = mat.size();
    int n = mat[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // dist[r][c] = steps to the nearest 0, -1 = not reached yet
    vector<vector<int>> dist(m, vector<int>(n, -1));
    queue<pair<int, int>> q;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (mat[r][c] != 0) { continue; }
            dist[r][c] = 0;
            q.push({r, c});
        }
    }
    while (!q.empty()) {
        auto [curR, curC] = q.front();
        q.pop();
        for (int d = 0; d < 4; d++) {
            int nxtR = curR + dirs[d][0];
            int nxtC = curC + dirs[d][1];
            if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
            if (dist[nxtR][nxtC] != -1) { continue; }
            dist[nxtR][nxtC] = dist[curR][curC] + 1;
            q.push({nxtR, nxtC});
        }
    }
    return dist;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (orangesRotting(grid) == want ? " PASS\n" : " FAIL\n");
}

void testNearest(const string& label, const vector<vector<int>>& mat,
                 const vector<vector<int>>& want) {
    cout << label << (nearestZeroDistance(mat) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("spread", {{2, 1, 1}, {0, 1, 0}}, 2);
    test("two-sources", {{2, 1, 1, 1, 2}}, 2);
    test("unreachable", {{2, 0, 1}}, -1);
    test("no-fresh", {{0, 2}}, 0);
    testNearest("nearest-corner", {{0, 1, 1}, {1, 1, 1}}, {{0, 1, 2}, {1, 2, 3}});
    testNearest("nearest-two-zeros", {{1, 0, 1, 1, 0}}, {{1, 0, 1, 1, 0}});
}

// follow-up: nearest-source distance (LC 542 / LC 286) -> nearestZeroDistance above
// follow-up: per-cell rot delays (a fresh orange rots delay[r][c] minutes after its
//   first rotten neighbor) -> multi-source Dijkstra on time instead of layers:
//   priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>,
//                  greater<tuple<int, int, int>>> pq;  // (time, r, c), earliest first
//   push every rotten cell with time 0 and best[r][c] = 0; pop; skip if time >
//   best[r][c]; for a fresh neighbor t = time + delay[nxtR][nxtC], if t < best, set and
//   push. Answer = max best over fresh cells, -1 if one is never reached.
