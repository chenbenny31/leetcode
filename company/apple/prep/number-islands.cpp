// count islands (connected 1's) in a grid of 0/1

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

void dfs(vector<vector<char>>& grid, int r, int c) {
    int m = grid.size();
    int n = grid[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != '1') { return; }
    
    grid[r][c] = '2'; // mark visited
    dfs(grid, r + 1, c);
    dfs(grid, r - 1, c);
    dfs(grid, r, c + 1);
    dfs(grid, r, c - 1);
}

// DFS flood fill per island, T: O(mn), S: O(mn)
int numIslands(vector<vector<char>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    int count = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == '1') {
                count++;
                dfs(grid, r, c);
            }
        }
    }
    // optional: restore back '2' to '1'
    return count;
}

// DFS with explicit stack re-used, T: O(mn), S: O(mn)
int numIslandsIter(vector<vector<char>> grid) {
    int m = grid.size();
    int n = grid[0].size();
    int dr[] = {0, 0, +1, -1};
    int dc[] = {+1, -1, 0, 0};
    vector<pair<int, int>> stk; // land cell already sunk
    int cout = 0;

    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] != '1') { continue; }
            count++;
            grid[r][c] = '2'; // sink on push
            stk.push_back({r, c});
            while (!stk.empty()) {
                auto [cr, cc] = stk.back();
                stk.pop_back();
                for (int d = 0; d < 4; d++) {
                    int nr = cr + dr[d];
                    int nc = cc + dc[d];
                    if (nr < 0 || nr >= m || nc < 0 || nr >= n || grid[nr][nc] != '1') {
                        continue;
                    }
                    grid[nr][nc] = '2';
                    stk.push_back({nr, nc});
                }
            }
        }
    }
    return count;
}

void test(const string& label, const vector<vector<char>>& grid, int want) {
    cout << label << (numIslands(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex2", {{'1', '1', '0', '0', '0'},
                    {'1', '1', '0', '0', '0'},
                    {'0', '0', '1', '0', '0'},
                    {'0', '0', '0', '1', '1'}}, 3);
    test("diagonal", {{'1', '0'}, {'0', '1'}}, 2);
    test("all-water", {{'0'}}, 0);
}
