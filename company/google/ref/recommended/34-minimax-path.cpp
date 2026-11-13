// 34 · Swim in Rising Water (Minimax Path) — LC 778, 1631
// Mother problem: select/09-advanced-graphs/04-swim-rising-water.cpp
//
// Google reports: LC 778, minimax path, safest path grid (35)
//
// LC 778: n x n grid of distinct elevations 0..n*n-1. At time t you can swim between
// 4-adjacent cells of elevation <= t. Return the min t to get from (0, 0) to (n-1, n-1).
//
// Clarify
// - the start cell counts, so the answer >= grid[0][0]; 1 x 1 grid → grid[0][0]
// - return the smallest possible max elevation on a path
//
// Core idea
// - Dijkstra where a path costs its max elevation; pops are non-decreasing, so the
//   first pop of the corner is the answer.
// - Cost sits on cells, so marking on push is safe here.
// - T O(n^2 log n), S O(n^2).
//
// Tests
// - single cell; down then right → 2; climb over a ridge → 5; non-consecutive heights
//
// Follow-ups
// - min effort path (LC 1631) → same Dijkstra with cost = max |height step|; cost sits
//   on edges, so keep an effort[r][c] table and skip stale pops instead of marking
// - binary search on the answer → binary search h, BFS through cells <= h to the corner
// - union-find by time → open cells in elevation order, unite open neighbors, stop when
//   the corners connect

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::greater;
using std::max;

// Dijkstra where a path's cost is its max elevation. T: O(n^2 log n), S: O(n^2).
int swimInWater(const vector<vector<int>>& grid) {
    int n = grid.size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // seen[r][c] = cell already pushed
    vector<vector<bool>> seen(n, vector<bool>(n, false));
    // (max elevation on the path, r * n + c), smallest first
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({grid[0][0], 0});
    seen[0][0] = true;
    while (!pq.empty()) {
        auto [pathMax, cell] = pq.top();
        pq.pop();
        int r = cell / n;
        int c = cell % n;
        if (r == n - 1 && c == n - 1) { return pathMax; }
        for (int d = 0; d < 4; d++) {
            int nxtR = r + dirs[d][0];
            int nxtC = c + dirs[d][1];
            if (nxtR < 0 || nxtR >= n || nxtC < 0 || nxtC >= n) { continue; }
            if (seen[nxtR][nxtC]) { continue; }
            // mark on push: pops are non-decreasing, so a later push is never better
            seen[nxtR][nxtC] = true;
            pq.push({max(pathMax, grid[nxtR][nxtC]), nxtR * n + nxtC});
        }
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (swimInWater(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {{0}}, 0);
    test("down-then-right", {{0, 3}, {1, 2}}, 2);
    test("must-climb", {{0, 5, 1},
                        {6, 7, 2},
                        {8, 4, 3}}, 5);
    test("height-gaps", {{10, 50}, {20, 30}}, 30);
}
