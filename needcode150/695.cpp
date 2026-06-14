// dfs in-place marking, T: O(mn), S: O(mn) stack frames

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int maxAreaOfIsland(std::vector<std::vector<int>>& grid) {
        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());
        int best = 0;

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (grid[r][c] == 1) { best = std::max(best, dfs(grid, r, c, m, n)); }
            }
        }
        return best;
    }

private:
    int dfs(std::vector<std::vector<int>>& grid,
            int r, int c, int m, int n) {
        if (r < 0 || r >= m || c < 0 || c >= n) { return 0; }
        if (grid[r][c] != 1) { return 0; }
        grid[r][c] = 2; // mark visited
        return 1 + dfs(grid, r + 1, c, m, n)
                 + dfs(grid, r - 1, c, m, n)
                 + dfs(grid, r, c + 1, m, n)
                 + dfs(grid, r, c - 1, m, n);
    }
};

// return value accumulation: post-roder, each contributes 1
// iterative dfs: pre-reserved std::stack<pair, vector>
