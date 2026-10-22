// LC 200 Number of Islands: count 4-directionally connected groups of '1' (land) in a
// grid of '0' (water) and '1'.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::pair;

// Scan; each unsunk land cell starts one island, BFS sinks it. T: O(R*C), S: O(R*C).
int numIslands(vector<vector<char>>& grid) {
    int m = grid.size(), n = grid[0].size();
    const int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    int islands = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != '1') { continue; }
            islands++;
            queue<pair<int, int>> q;   // land cells of this island not expanded yet
            grid[r][c] = '0';
            q.push({r, c});
            while (!q.empty()) {
                auto [cr, cc] = q.front();
                q.pop();
                for (const auto& d : dirs) {
                    int nr = cr + d[0], nc = cc + d[1];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                    if (grid[nr][nc] != '1') { continue; }
                    // sink on push so a cell is queued once
                    grid[nr][nc] = '0';
                    q.push({nr, nc});
                }
            }
        }
    }
    return islands;
}

void test(const string& label, vector<vector<char>> grid, int want) {
    cout << label << (numIslands(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diagonal-apart", {{'1', '0'}, {'0', '1'}}, 2);
    test("one-blob", {{'1', '1', '0'}, {'0', '1', '0'}, {'0', '1', '1'}}, 1);
    test("all-water", {{'0', '0'}}, 0);
}
