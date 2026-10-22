// LC 1293 Shortest Path in a Grid with Obstacles Elimination: m x n grid, 0 = empty and
// 1 = obstacle. From (0, 0) to (m-1, n-1) in 4 directions, you may remove up to k
// obstacles. Return the minimum number of steps, or -1 if impossible.
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

// BFS over states (row, col, removals left). T: O(m*n*k), S: O(m*n*k).
int shortestPath(vector<vector<int>>& grid, int k) {
    int m = grid.size(), n = grid[0].size();
    // dist[r][c][left] = steps to reach (r, c) with `left` removals unused, -1 = unseen
    vector<vector<vector<int>>> dist(m, vector<vector<int>>(n, vector<int>(k + 1, -1)));
    queue<tuple<int, int, int>> q;   // states in non-decreasing steps
    dist[0][0][k] = 0;
    q.push({0, 0, k});
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    while (!q.empty()) {
        auto [r, c, left] = q.front();
        q.pop();
        if (r == m - 1 && c == n - 1) { return dist[r][c][left]; }
        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            int nl = left - grid[nr][nc];
            if (nl < 0 || dist[nr][nc][nl] != -1) { continue; }
            dist[nr][nc][nl] = dist[r][c][left] + 1;
            q.push({nr, nc, nl});
        }
    }
    return -1;
}

void test(const string& label, vector<vector<int>> grid, int k, int want) {
    cout << label << (shortestPath(grid, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("break-wall", {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}, 1, 4);
    test("walled-off", {{0, 1, 0}, {0, 1, 0}, {0, 1, 0}}, 0, -1);
    test("must-break", {{0, 1}, {1, 0}}, 1, 2);
    test("not-enough", {{0, 1, 1}, {1, 1, 0}}, 1, -1);
}
