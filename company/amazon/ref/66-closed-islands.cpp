// LC 1254: in a grid 0 = land and 1 = water. A closed island is a 4-connected group of
// land that does not touch the grid border. Return the number of closed islands.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// turn the land group containing (r, c) into water
void dfs(vector<vector<int>>& grid, int r, int c) {
    int m = grid.size(), n = grid[0].size();
    if (r < 0 || r >= m || c < 0 || c >= n || grid[r][c] != 0) { return; }
    grid[r][c] = 1;
    dfs(grid, r + 1, c);
    dfs(grid, r - 1, c);
    dfs(grid, r, c + 1);
    dfs(grid, r, c - 1);
}

// First sink every island touching the border; what is left is closed.
// T: O(mn), S: O(mn) recursion in the worst case.
int closedIsland(vector<vector<int>> grid) {
    int m = grid.size(), n = grid[0].size();
    for (int r = 0; r < m; r++) {
        dfs(grid, r, 0);
        dfs(grid, r, n - 1);
    }
    for (int c = 0; c < n; c++) {
        dfs(grid, 0, c);
        dfs(grid, m - 1, c);
    }
    int count = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (grid[r][c] == 0) {
                count++;
                dfs(grid, r, c);
            }
        }
    }
    return count;
}

void test(const string& label, const vector<vector<int>>& grid, int want) {
    cout << label << (closedIsland(grid) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one", {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}, 1);
    test("touches-edge", {{1, 0, 1}, {1, 0, 1}, {1, 1, 1}}, 0);
    test("two", {{1, 1, 1, 1, 1}, {1, 0, 1, 0, 1}, {1, 1, 1, 1, 1}}, 2);
    test("all-land", {{0, 0}, {0, 0}}, 0);
}
