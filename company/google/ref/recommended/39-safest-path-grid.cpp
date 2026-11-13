// LC 2812 Find the Safest Path in a Grid (Google-tagged)
// n x n grid, 1 = thief, 0 = empty, 4-dir moves; safeness = min dist to a thief on path
// return max safeness over paths (0, 0) -> (n-1, n-1)
// reported: also as "path maximizing the min distance to a cat"
//
// clarify:
// thief cells -> may be crossed, safeness 0
// no walls -> BFS steps = Manhattan dist
// start / end cells -> count in the min
// n up to 400 -> O(n^2 log n) fine
// return -> safeness only; path in follow-up

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

// multi-source BFS w/ every thief enqueued at 0; O(n^2)
vector<vector<int>> thiefDistances(const vector<vector<int>>& grid) {
    int n = grid.size();
    // dist[r][c] = Manhattan dist to nearest thief, -1 if not set
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

// solution 1: BFS + maximin Dijkstra; also gives the path via parents
// core idea: thief dist by multi-source BFS, then max-heap on min dist along the path
// first pop of a cell is final: extending a path never raises its min
// T: O(n^2 log n), S: O(n^2)
int maximumSafenessFactor(const vector<vector<int>>& grid) {
    int n = grid.size();
    vector<vector<int>> dist = thiefDistances(grid);
    // best[r][c] = largest safeness of a path (0, 0) -> (r, c) so far, -1 if none
    vector<vector<int>> best(n, vector<int>(n, -1));
    // {path safeness, r, c}, largest safeness on top
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

// BFS from (0, 0) entering only cells w/ dist >= limit; O(n^2)
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

// solution 2: BFS + binary search on the answer; no heap, easy to explain
// core idea: limit reachable -> every smaller one is too; binary search largest ok limit
// T: O(n^2 log n), S: O(n^2)
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

// follow-up:
// return the path -> record parent cell on each Dijkstra improvement, walk back from end
// union-find alternative -> open cells from safest down; answer = dist when corners join
// walls in grid -> BFS skips walls (dist = path len), Dijkstra never enters; may be -1
// BFS per thief -> O(thieves * n^2); seed one BFS w/ every thief instead
