// LC 1091 Shortest Path in Binary Matrix: in an n x n 0/1 grid, find the length of the
// shortest clear path (cells with 0, 8-directional moves) from top-left to bottom-right.
// The length counts cells on the path; return -1 if there is none.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::queue;

// BFS: every move costs 1, so a cell's first discovery is its shortest distance.
// T: O(n^2), S: O(n^2).
int shortestPathBinaryMatrix(vector<vector<int>>& grid) {
    int n = grid.size();
    if (grid[0][0] == 1 || grid[n - 1][n - 1] == 1) { return -1; }
    // dist[r][c] = cells on the shortest path to (r, c), 0 = unseen
    vector<vector<int>> dist(n, vector<int>(n, 0));
    queue<pair<int, int>> q;                          // cells in non-decreasing distance
    dist[0][0] = 1;
    q.push({0, 0});
    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();
        if (r == n - 1 && c == n - 1) { return dist[r][c]; }
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                int nr = r + dr, nc = c + dc;
                if (nr < 0 || nr >= n || nc < 0 || nc >= n) { continue; }
                if (grid[nr][nc] == 1 || dist[nr][nc] != 0) { continue; }
                dist[nr][nc] = dist[r][c] + 1;   // mark on push so a cell is queued once
                q.push({nr, nc});
            }
        }
    }
    return -1;
}

void test(const string& label, vector<vector<int>> grid, int want) {
    cout << label << (shortestPathBinaryMatrix(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diagonal", {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}, 3);
    test("single-cell", {{0}}, 1);
    test("blocked-start", {{1, 0}, {0, 0}}, -1);
    test("walled-off", {{0, 1}, {1, 1}}, -1);
}
