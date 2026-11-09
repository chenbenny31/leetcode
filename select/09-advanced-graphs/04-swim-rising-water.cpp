// LC 778 Swim in Rising Water (Hard). Pattern: Dijkstra on max elevation (bottleneck).
// n x n grid of distinct elevations 0..n*n-1. At time t you can swim between 4-adjacent
// cells of elevation <= t. Return the min t to get from (0, 0) to (n-1, n-1).

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
}

// follow-up: binary search t + BFS through cells <= t -> monotone in t, O(n^2 log n)
// follow-up: union-find by time -> open cells in elevation order, unite open neighbors,
//   stop when (0, 0) and (n-1, n-1) connect; O(n^2 alpha)
