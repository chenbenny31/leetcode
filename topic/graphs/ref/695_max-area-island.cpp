// LC 695 Max Area of Island: in a grid of 0 (water) and 1 (land), return the largest
// area of a 4-directionally connected island, or 0 if there is none.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// size of the island containing (r, c); sinks every cell it counts
int dfs(vector<vector<int>>& grid, int r, int c) {
    int m = grid.size(), n = grid[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] == 0) { return 0; }
    grid[r][c] = 0;
    return 1 + dfs(grid, r + 1, c) + dfs(grid, r - 1, c) + dfs(grid, r, c + 1) +
           dfs(grid, r, c - 1);
}

// Try DFS from every cell; sunk cells are never counted twice. T: O(R*C), S: O(R*C).
int maxAreaOfIsland(vector<vector<int>>& grid) {
    int m = grid.size(), n = grid[0].size();
    int best = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            best = max(best, dfs(grid, r, c));
        }
    }
    return best;
}

void test(const string& label, vector<vector<int>> grid, int want) {
    cout << label << (maxAreaOfIsland(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-islands", {{1, 1, 0, 1}, {0, 1, 0, 0}, {0, 0, 0, 1}}, 3);
    test("diagonal-apart", {{1, 0}, {0, 1}}, 1);
    test("all-water", {{0, 0}}, 0);
    test("all-land", {{1, 1}, {1, 1}}, 4);
}
