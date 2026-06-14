// dfs from border, T: O(mn), S: O(mn) stack frames

class Solution {
public:
    int numEnclaves(std::vector<std::vector<int>>& grid) {
        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());

        // flood fill from all border land cells
        for (int r = 0; r < m; r++) {
            dfs(grid, r, 0, m, n);
            dfs(grid, r, n - 1, m, n);
        }
        for (int c = 0; c < n; c++) {
            dfs(grid, 0, c, m, n);
            dfs(grid, m - 1, c, m, n);
        }

        // count remaining land cells
        int count = 0;
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (grid[r][c] == 1) { count++; }
            }
        }
        return count;
    }

private:
    void dfs(std::vector<std::vector<int>>& grid,
            int r, int c, int m, int n) {
        if (r < 0 || r >= m || c < 0 || c >= n) { return; }
        if (grid[r][c] != 1) { return; }
        grid[r][c] = 2;
        dfs(grid, r + 1, c, m, n);
        dfs(grid, r - 1, c, m, n);
        dfs(grid, r, c + 1, m, n);
        dfs(grid, r, c - 1, m, n);
    }
};
