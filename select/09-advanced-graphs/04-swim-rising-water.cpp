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
using std::queue;
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

// BFS from (0, 0) through cells of height <= h; true if it reaches the far corner.
bool canReach(const vector<vector<int>>& grid, int h) {
    int m = grid.size();
    int n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // seen[r][c] = cell already queued
    vector<vector<bool>> seen(m, vector<bool>(n, false));
    // reachable cells still to expand
    queue<pair<int, int>> q;
    q.push({0, 0});
    seen[0][0] = true;
    while (!q.empty()) {
        auto [curR, curC] = q.front();
        q.pop();
        if (curR == m - 1 && curC == n - 1) { return true; }
        for (int d = 0; d < 4; d++) {
            int nxtR = curR + dirs[d][0];
            int nxtC = curC + dirs[d][1];
            if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
            if (seen[nxtR][nxtC] || grid[nxtR][nxtC] > h) { continue; }
            seen[nxtR][nxtC] = true;
            q.push({nxtR, nxtC});
        }
    }
    return false;
}

// J44: binary search the answer h over [grid[0][0], max height]; reachable at h stays
// reachable at h + 1, so find the first h where canReach holds.
// T: O(n^2 log(maxHeight)), S: O(n^2).
int minimaxBinarySearch(const vector<vector<int>>& grid) {
    // the start cell is on every path
    int lo = grid[0][0];
    int hi = grid[0][0];
    for (const vector<int>& row : grid) {
        for (int height : row) { hi = max(hi, height); }
    }
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (canReach(grid, mid)) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    bool ok = swimInWater(grid) == want && minimaxBinarySearch(grid) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {{0}}, 0);
    test("down-then-right", {{0, 3}, {1, 2}}, 2);
    test("must-climb", {{0, 5, 1},
                        {6, 7, 2},
                        {8, 4, 3}}, 5);
    test("height-gaps", {{10, 50}, {20, 30}}, 30);
}

// follow-up: union-find by time -> open cells in elevation order, unite open neighbors,
//   stop when (0, 0) and (n-1, n-1) connect; O(n^2 alpha)
