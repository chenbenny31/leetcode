// LC 200: grid of '1' (land) and '0' (water); count islands, where an island is land
// connected horizontally or vertically.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;
using std::pair;

// straightforward: recursive DFS flood fill; sink land cell (r, c) and all land
// connected to it.
// T: O(m*n) over all calls, S: O(m*n) recursion depth in the worst case.
void dfs(vector<vector<char>>& grid, int r, int c) {
    int m = grid.size(), n = grid[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != '1') { return; }
    grid[r][c] = '0';
    dfs(grid, r + 1, c);
    dfs(grid, r - 1, c);
    dfs(grid, r, c + 1);
    dfs(grid, r, c - 1);
}

// each unvisited land cell starts a new island. T: O(m*n), S: O(m*n).
int numIslands(vector<vector<char>> grid) {
    int m = grid.size(), n = grid[0].size();
    int islands = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == '1') {
                islands++;
                dfs(grid, r, c);
            }
        }
    }
    return islands;
}

// improved: same DFS order with an explicit stack reused across islands: no recursion
// depth limit, no call overhead, no per-island allocation. T: O(m*n), S: O(m*n).
int numIslandsIter(vector<vector<char>> grid) {
    int m = grid.size(), n = grid[0].size();
    int dirs[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    vector<pair<int, int>> stk;   // land cells already sunk, neighbors not yet checked
    int islands = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != '1') { continue; }
            islands++;
            // sink on push, so no cell is pushed twice
            grid[r][c] = '0';
            stk.push_back({r, c});
            while (!stk.empty()) {
                auto [cr, cc] = stk.back();
                stk.pop_back();
                for (auto& d : dirs) {
                    int nr = cr + d[0], nc = cc + d[1];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n || grid[nr][nc] != '1') {
                        continue;
                    }
                    grid[nr][nc] = '0';
                    stk.push_back({nr, nc});
                }
            }
        }
    }
    return islands;
}

void test(const string& label, const vector<vector<char>>& grid, int want) {
    bool ok = numIslands(grid) == want && numIslandsIter(grid) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two", {{'1', '1', '0'},
                 {'0', '0', '0'},
                 {'0', '1', '1'}}, 2);
    test("diag", {{'1', '0'}, {'0', '1'}}, 2);
    test("water", {{'0'}}, 0);
}
