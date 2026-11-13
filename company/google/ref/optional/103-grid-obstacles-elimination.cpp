// 103 · Shortest Path in a Grid with Obstacles Elimination — LC 1293
// Mother: select/09-advanced-graphs/03-grid-obstacles-elimination.cpp
// m x n grid, 0 = empty, 1 = obstacle; walk 4-dir from (0, 0) to (m-1, n-1)
// may remove <= k obstacles; return min steps, or -1 if impossible
// reported: grid shortest path w/ k eliminations x2 (FastPrep)
//
// clarify:
// start / end cells -> empty
// unreachable -> -1
// 1 x 1 grid -> 0 steps

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::tuple;

// core idea: BFS over states (row, col, removals left); stepping onto a 1 costs 1 removal
// unit edges, so the first pop of the target is optimal
// T: O(V + E), S: O(V) — V = m * n * (k + 1) states, E <= 4V
int shortestPath(const vector<vector<int>>& grid, int k) {
    int m = grid.size();
    int n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // dist[r][c][left] = steps to reach (r, c) w/ `left` removals unused, -1 = unseen
    vector<vector<vector<int>>> dist(m, vector<vector<int>>(n, vector<int>(k + 1, -1)));
    queue<tuple<int, int, int>> q;
    dist[0][0][k] = 0;
    q.push({0, 0, k});
    while (!q.empty()) {
        auto [r, c, left] = q.front();
        q.pop();
        if (r == m - 1 && c == n - 1) { return dist[r][c][left]; }
        for (int d = 0; d < 4; d++) {
            int nxtR = r + dirs[d][0];
            int nxtC = c + dirs[d][1];
            if (nxtR < 0 || nxtR >= m || nxtC < 0 || nxtC >= n) { continue; }
            // stepping onto an obstacle uses one removal
            int nxtLeft = left - grid[nxtR][nxtC];
            if (nxtLeft < 0 || dist[nxtR][nxtC][nxtLeft] != -1) { continue; }
            dist[nxtR][nxtC][nxtLeft] = dist[r][c][left] + 1;
            q.push({nxtR, nxtC, nxtLeft});
        }
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& grid, int k, int want) {
    cout << label << (shortestPath(grid, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("break-wall", {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}, 1, 4);
    test("walled-off", {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}, 0, -1);
    test("not-enough", {{0, 1, 1}, {1, 1, 0}}, 1, -1);
    test("single", {{0}}, 0, 0);
}

// follow-up:
// prune -> if k >= m + n - 2, return m + n - 2 at once (a Manhattan path is free)
// weighted cells -> Dijkstra on the same (r, c, left) state
// one wall break only -> grid shortest path one break (06) is the k = 1 case
