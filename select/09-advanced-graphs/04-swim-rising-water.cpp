// LC 778 Swim in Rising Water (Hard) — Dijkstra on the max elevation of a path
// n x n grid of distinct elevations 0..n*n-1. At time t you can swim between 4-adjacent
// cells of elevation <= t. Return the least t to get from (0, 0) to (n-1, n-1).
// Clarify: the start cell counts (answer >= grid[0][0]); swimming takes no time;
//   1 x 1 grid; ranges (n <= 50).

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <functional>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::greater;
using std::max;

// Core idea: Dijkstra where a path costs its highest cell; always expand the cell with
// the lowest path max, and the first pop of the target is the answer.
// T: O(n^2 log n), S: O(n^2).
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
    test("must-climb", {{0, 5, 1}, {6, 7, 2}, {8, 4, 3}}, 5);
    test("height-gaps", {{10, 50}, {20, 30}}, 30);
    return 0;
}

// follow-up (ideas only):
// - binary search the answer -> reachable at height h stays reachable at h + 1, so
//   search h in [grid[0][0], max] with a BFS over cells <= h; O(n^2 log(maxHeight))
// - union-find by time -> open the cells in elevation order, unite each with its open
//   neighbors, stop when (0, 0) and (n-1, n-1) share a root
// - path with minimum effort (LC 1631) -> the cost is on edges (|height difference|):
//   keep effort[r][c], relax on strict improvement and skip stale pops; marking on push
//   is wrong there
// - path with maximum minimum value (LC 1102) -> the mirror: max-heap on the path min
