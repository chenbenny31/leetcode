// LC 2812 Find the Safest Path in a Grid (Google-tagged; also reported as "path
// maximizing the minimum distance to a cat"). n x n grid, grid[r][c] = 1 thief, 0 empty;
// at least one thief. Moves go up / down / left / right and may cross thief cells.
// The safeness of a path = min over its cells of the Manhattan distance to the nearest
// thief. Return the max safeness over all paths (0, 0) -> (n - 1, n - 1).
//
// Clarify:
// - 4 directions; thief cells may be crossed (their own safeness is 0)
// - no walls, so BFS steps from the nearest thief equal the Manhattan distance
// - at least one thief (given), so every cell gets a distance
// - the start and the end cell count in the min
// - n up to 400 -> 1.6e5 cells: one multi-source BFS, then O(n^2 log n)
// - return the safeness only (the path is a follow-up)
// Idea: (1) multi-source BFS from all thieves gives dist[r][c]. (2) Maximize the
//   bottleneck min(dist) along a path: Dijkstra with a max-heap on the bottleneck; a
//   cell's first pop is final since extending a path never raises its min. Or binary
//   search the answer: "a path with every dist >= limit" is a plain BFS.
// Pitfalls:
// - one BFS seeded with every thief, not a BFS per thief (O(thieves * n^2))
// - max-heap here (the default priority_queue), unlike shortest-path Dijkstra
// - nxtSafe = min(safe, dist[nxt]), not a sum; seed best[0][0] = dist[0][0], which
//   caps the answer
// - binary search for the largest limit: mid rounds up (lo + (hi - lo + 1) / 2), else
//   lo = mid loops forever; canKeep must test the start cell itself
// Tests to write: one cell (a thief) -> 0; thief at the start -> 0; a detour around a
//   corner thief; a thief in the middle of 5 x 5 -> go around it at safeness 2.

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
using std::max;

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

// root of cell id, halving the path on the way up
int findRoot(vector<int>& parent, int id) {
    while (parent[id] != id) {
        parent[id] = parent[parent[id]];
        id = parent[id];
    }
    return id;
}

// follow-up (union-find): open cells from the safest down (bucketed by dist, which is
// at most 2n - 2), joining each with its open neighbors; the dist of the bucket after
// which (0, 0) and (n - 1, n - 1) share a root is the answer.
// Pitfall: test the join after the whole bucket, and only once both corners are open.
// T: O(n^2 log n) with path halving alone, S: O(n^2).
int maximumSafenessFactorUnionFind(const vector<vector<int>>& grid) {
    int n = grid.size();
    vector<vector<int>> dist = thiefDistances(grid);
    int maxDist = 0;
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) { maxDist = max(maxDist, dist[r][c]); }
    }
    // buckets[d] = ids r * n + c of the cells at distance d
    vector<vector<int>> buckets(maxDist + 1);
    for (int r = 0; r < n; r++) {
        for (int c = 0; c < n; c++) { buckets[dist[r][c]].push_back(r * n + c); }
    }
    // parent[id] = parent cell of id in the union-find, -1 = not open yet
    vector<int> parent(n * n, -1);
    int last = n * n - 1;
    for (int d = maxDist; d >= 0; d--) {
        for (int id : buckets[d]) {
            parent[id] = id;
            int curR = id / n;
            int curC = id % n;
            for (int k = 0; k < 4; k++) {
                int nxtR = curR + dirs[k][0];
                int nxtC = curC + dirs[k][1];
                if (nxtR < 0 || nxtR >= n || nxtC < 0 || nxtC >= n) { continue; }
                int nxt = nxtR * n + nxtC;
                if (parent[nxt] == -1) { continue; }
                parent[findRoot(parent, id)] = findRoot(parent, nxt);
            }
        }
        if (parent[0] == -1 || parent[last] == -1) { continue; }
        if (findRoot(parent, 0) == findRoot(parent, last)) { return d; }
    }
    return 0;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    bool ok = maximumSafenessFactor(grid) == want;
    ok = ok && maximumSafenessFactorBinary(grid) == want;
    ok = ok && maximumSafenessFactorUnionFind(grid) == want;
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
    test("two-thieves-gap", {{0, 0, 0, 0},
                             {1, 0, 0, 0},
                             {0, 0, 0, 1},
                             {0, 0, 0, 0}}, 1);
}

// follow-up: return the path too -> in the Dijkstra, record where each improvement
//   came from and walk back from the end:
//   parent[nxtR][nxtC] = curR * n + curC;   // next to best[nxtR][nxtC] = nxtSafe
//   for (int id = n * n - 1; id != -1; id = parent[id / n][id % n]) {
//       path.push_back({id / n, id % n});
//   }
//   reverse(path.begin(), path.end());   // parent starts as all -1
// follow-up: union-find alternative -> maximumSafenessFactorUnionFind above
// follow-up: walls in the grid -> step 1 BFS skips walls (distance becomes path length,
//   not Manhattan), step 2 never enters a wall, and an unreachable end returns -1:
//   if (grid[nxtR][nxtC] == 2) { continue; }   // in the BFS and the Dijkstra loops
