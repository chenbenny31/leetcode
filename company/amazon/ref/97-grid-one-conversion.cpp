// Grid cells are 0 (open) or 1 (blocked); moves are 4-directional. You may turn at most
// one blocked cell into an open one. Return the fewest moves from start to target, or -1.
// (hack2hire's version leaves start/target unclear; this is the common form, LC 1293
// with k = 1.)

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <array>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::array;
using std::pair;

// BFS over states (row, col, used) where used = whether the conversion is spent; a
// blocked neighbor is enterable only with used == 0. T: O(mn), S: O(mn).
int shortestPath(const vector<vector<int>>& grid, pair<int, int> start,
                 pair<int, int> target) {
    int m = grid.size(), n = grid[0].size();
    auto [sr, sc] = start;
    auto [tr, tc] = target;
    if (grid[sr][sc] == 1) { return -1; }
    // dist[r][c][u] = moves to reach (r, c) having used u conversions, -1 if unseen
    vector<vector<array<int, 2>>> dist(m, vector<array<int, 2>>(n, {-1, -1}));
    int dr[4] = {1, -1, 0, 0}, dc[4] = {0, 0, 1, -1};
    queue<array<int, 3>> q;
    dist[sr][sc][0] = 0;
    q.push({sr, sc, 0});
    while (!q.empty()) {
        auto [r, c, used] = q.front();
        q.pop();
        int d = dist[r][c][used];
        if (r == tr && c == tc) { return d; }
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            int nu = used + grid[nr][nc];
            if (nu > 1 || dist[nr][nc][nu] != -1) { continue; }
            dist[nr][nc][nu] = d + 1;
            q.push({nr, nc, nu});
        }
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& grid, pair<int, int> start,
          pair<int, int> target, int want) {
    cout << label << (shortestPath(grid, start, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("open", {{0, 0, 0}}, {0, 0}, {0, 2}, 2);
    test("break-wall", {{0, 1, 0}}, {0, 0}, {0, 2}, 2);
    test("two-walls", {{0, 1, 1, 0}}, {0, 0}, {0, 3}, -1);
    test("detour-vs-break", {{0, 1, 0}, {0, 0, 0}}, {0, 0}, {0, 2}, 2);
    test("same-cell", {{0}}, {0, 0}, {0, 0}, 0);
}
