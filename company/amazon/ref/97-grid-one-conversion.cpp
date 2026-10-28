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
    if (grid[start.first][start.second] == 1) { return -1; }
    // dist[r][c][u] = moves to reach (r, c) having used u conversions, -1 if unseen
    vector<vector<array<int, 2>>> dist(m, vector<array<int, 2>>(n, {-1, -1}));
    int dr[4] = {1, -1, 0, 0}, dc[4] = {0, 0, 1, -1};
    queue<array<int, 3>> q;
    dist[start.first][start.second][0] = 0;
    q.push({start.first, start.second, 0});
    while (!q.empty()) {
        array<int, 3> cur = q.front();
        q.pop();
        int d = dist[cur[0]][cur[1]][cur[2]];
        if (cur[0] == target.first && cur[1] == target.second) { return d; }
        for (int i = 0; i < 4; i++) {
            int r = cur[0] + dr[i], c = cur[1] + dc[i];
            if (r < 0 || r >= m || c < 0 || c >= n) { continue; }
            int used = cur[2] + grid[r][c];
            if (used > 1 || dist[r][c][used] != -1) { continue; }
            dist[r][c][used] = d + 1;
            q.push({r, c, used});
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
