// LC 2812 Find the Safest Path in a Grid (Google-tagged; also reported as "path
// maximizing the minimum distance to a cat"). n x n grid, grid[r][c] = 1 thief, 0 empty;
// at least one thief. Moves go up / down / left / right and may cross thief cells.
// The safeness of a path = min over its cells of the Manhattan distance to the nearest
// thief. Return the max safeness over all paths (0, 0) -> (n - 1, n - 1).

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <tuple>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::priority_queue;
using std::tuple;
using std::pair;
using std::min;

const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

// step 1: multi-source BFS with every thief enqueued at 0; with no walls the BFS
// distance equals the Manhattan distance to the nearest thief. T: O(n^2), S: O(n^2).
vector<vector<int>> thiefDistances(const vector<vector<int>>& grid) {
    int n = grid.size();
    // dist[r][c] = Manhattan distance from (r, c) to the nearest thief, -1 if not set
    vector<vector<int>> dist(n, vector<int>(n, -1));
    queue<pair<int, int>> q;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 0) { continue; }
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
            if (nxtR < 0 || nxtR >= n || nxtC < 0 || nxtC >= n) { continue; }
            if (dist[nxtR][nxtC] != -1) { continue; }
            dist[nxtR][nxtC] = dist[curR][curC] + 1;
            q.push({nxtR, nxtC});
        }
    }
    return dist;
}

// step 2: maximin Dijkstra with a max-heap on the bottleneck (min dist along the path);
// a cell's first pop has its best safeness, since extending a path never raises it.
// T: O(n^2 log n), S: O(n^2).
int maximumSafenessFactor(const vector<vector<int>>& grid) {
    int n = grid.size();
    vector<vector<int>> dist = thiefDistances(grid);
    // best[r][c] = largest safeness of a path (0, 0) -> (r, c) found so far, -1 if none
    vector<vector<int>> best(n, vector<int>(n, -1));
    // {safeness of the path to the cell, r, c}, largest safeness on top
    priority_queue<tuple<int, int, int>> heap;
    best[0][0] = dist[0][0];
    heap.push({dist[0][0], 0, 0});
    while (!heap.empty()) {
        auto [safe, curR, curC] = heap.top();
        heap.pop();
        if (curR == n - 1 && curC == n - 1) { return safe; }
        if (safe < best[curR][curC]) { continue; }
        for (int d = 0; d < 4; d++) {
            int nxtR = curR + dirs[d][0];
            int nxtC = curC + dirs[d][1];
            if (nxtR < 0 || nxtR >= n || nxtC < 0 || nxtC >= n) { continue; }
            int nxtSafe = min(safe, dist[nxtR][nxtC]);
            if (nxtSafe <= best[nxtR][nxtC]) { continue; }
            best[nxtR][nxtC] = nxtSafe;
            heap.push({nxtSafe, nxtR, nxtC});
        }
    }
    return 0;
}

// BFS from (0, 0) entering only cells with dist >= limit. T: O(n^2), S: O(n^2).
bool canKeep(const vector<vector<int>>& dist, int limit) {
    int n = dist.size();
    if (dist[0][0] < limit) { return false; }
    vector<vector<bool>> seen(n, vector<bool>(n, false));
    seen[0][0] = true;
    queue<pair<int, int>> q;
    q.push({0, 0});
    while (!q.empty()) {
        auto [curR, curC] = q.front();
        q.pop();
        if (curR == n - 1 && curC == n - 1) { return true; }
        for (int d = 0; d < 4; d++) {
            int nxtR = curR + dirs[d][0];
            int nxtC = curC + dirs[d][1];
            if (nxtR < 0 || nxtR >= n || nxtC < 0 || nxtC >= n) { continue; }
            if (seen[nxtR][nxtC] || dist[nxtR][nxtC] < limit) { continue; }
            seen[nxtR][nxtC] = true;
            q.push({nxtR, nxtC});
        }
    }
    return false;
}

// second version: if safeness `limit` is reachable, so is every smaller one, so binary
// search the largest limit canKeep accepts (limit 0 always works).
// T: O(n^2 log n), S: O(n^2).
int maximumSafenessFactorBinary(const vector<vector<int>>& grid) {
    int n = grid.size();
    vector<vector<int>> dist = thiefDistances(grid);
    int lo = 0;
    int hi = min(dist[0][0], dist[n - 1][n - 1]);
    while (lo < hi) {
        // round up so lo = mid always moves
        int mid = lo + (hi - lo + 1) / 2;
        if (canKeep(dist, mid)) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    bool ok = maximumSafenessFactor(grid) == want;
    ok = ok && maximumSafenessFactorBinary(grid) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("single", {{1}}, 0);
    test("thief-at-start", {{1, 0}, {0, 0}}, 0);
    test("detour-left", {{0, 0, 1}, {0, 0, 0}, {0, 0, 0}}, 2);
    test("ring", {{0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0},
                  {0, 0, 1, 0, 0},
                  {0, 0, 0, 0, 0},
                  {0, 0, 0, 0, 0}}, 2);
}

// follow-up: return the path too -> store parent[r][c] when best improves in Dijkstra
//   and walk back from (n - 1, n - 1)
// follow-up: union-find alternative -> add cells in decreasing dist order, union with
//   added neighbors; the dist at which (0, 0) and (n - 1, n - 1) join is the answer
// follow-up: walls in the grid -> step 1 BFS skips walls (distance becomes path length,
//   not Manhattan), step 2 never enters a wall; the rest is unchanged
