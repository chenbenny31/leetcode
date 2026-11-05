// ByteDance (hack2hire) / LC 778: n x n grid of distinct elevations 0..n*n-1. At time t
// you can swim between 4-adjacent cells of elevation <= t. Return the min t to get from
// (0, 0) to (n-1, n-1).

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;
using std::greater;
using std::max;

// Dijkstra where a path's cost is its max elevation. T: O(n^2 log n), S: O(n^2).
int swimInWater(const vector<vector<int>>& grid) {
    int n = grid.size();
    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};
    // seen[r][c] = cell already pushed
    vector<vector<bool>> seen(n, vector<bool>(n, false));
    // min-heap of (max elevation on the path, r * n + c)
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> heap;
    heap.push({grid[0][0], 0});
    seen[0][0] = true;
    while (!heap.empty()) {
        int t = heap.top().first;
        int r = heap.top().second / n;
        int c = heap.top().second % n;
        heap.pop();
        if (r == n - 1 && c == n - 1) {
            return t;
        }
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n || seen[nr][nc]) {
                continue;
            }
            // mark on push: pops come in non-decreasing t, so a later push is no better
            seen[nr][nc] = true;
            heap.push({max(t, grid[nr][nc]), nr * n + nc});
        }
    }
    return -1;
}

// BFS through cells of elevation <= t.
bool canReach(const vector<vector<int>>& grid, int t) {
    int n = grid.size();
    if (grid[0][0] > t) {
        return false;
    }
    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};
    // seen[r][c] = cell already queued
    vector<vector<bool>> seen(n, vector<bool>(n, false));
    queue<pair<int, int>> q;
    q.push({0, 0});
    seen[0][0] = true;
    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();
        if (r == n - 1 && c == n - 1) {
            return true;
        }
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr < 0 || nr >= n || nc < 0 || nc >= n || seen[nr][nc]) {
                continue;
            }
            if (grid[nr][nc] > t) {
                continue;
            }
            seen[nr][nc] = true;
            q.push({nr, nc});
        }
    }
    return false;
}

// follow-up: canReach(t) is monotone in t; binary search the smallest t.
// T: O(n^2 log n), S: O(n^2).
int swimInWaterBinary(const vector<vector<int>>& grid) {
    int n = grid.size();
    int lo = grid[0][0];
    // at t = n*n - 1 every cell is open
    int hi = n * n - 1;
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
    bool ok = swimInWater(grid) == want && swimInWaterBinary(grid) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {{0}}, 0);
    test("lc-ex1", {{0, 2}, {1, 3}}, 3);
    test("lc-ex2", {{0, 1, 2, 3, 4},
                    {24, 23, 22, 21, 5},
                    {12, 13, 14, 15, 16},
                    {11, 17, 18, 19, 20},
                    {10, 9, 8, 7, 6}}, 16);
}

// follow-up: binary search t + BFS -> see swimInWaterBinary() above; O(n^2 log n)
// follow-up: union-find by time -> pos[t] = cell with elevation t; open cells in order,
//   unite open neighbors, stop when find(0) == find(n*n - 1); O(n^2 alpha)
// follow-up: return the path -> parent[cell] set on push, walk back from n*n - 1
// follow-up: min effort path (LC 1631) -> same Dijkstra, cost = max |height diff| on edge
