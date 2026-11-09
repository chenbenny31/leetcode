// LC 1293 Shortest Path in a Grid with Obstacles Elimination (Hard). Pattern: BFS on
// (r, c, k) state. m x n grid, 0 = empty, 1 = obstacle; walk 4-directionally from (0, 0)
// to (m-1, n-1) removing up to k obstacles. Return the min steps, or -1 if impossible.

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
int shortestPath(const vector<vector<int>>& grid, int k) {
    int m = grid.size();
    int n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    // dist[r][c][left] = steps to reach (r, c) with `left` removals unused, -1 = unseen
    vector<vector<vector<int>>> dist(m, vector<vector<int>>(n, vector<int>(k + 1, -1)));
    // states in non-decreasing steps
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

// follow-up: prune -> if k >= m + n - 2, return m + n - 2 at once (Manhattan path)
// follow-up: weighted cells -> Dijkstra on the same (r, c, left) state
