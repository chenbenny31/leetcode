// reverse dfs from border, T: O(mn), S: O(mn)

#include <vector>
#include <cstdint>

class Solution {
public:
    std::vector<std::vector<int>> pacificAtlantic(std::vector<std::vector<int>>& heights) {
        int m = static_cast<int>(heights.size());
        int n = static_cast<int>(heights[0].size());

        std::vector<std::vector<uint8_t>> pac(m, std::vector<uint8_t>(n, 0));
        std::vector<std::vector<uint8_t>> atl(m, std::vector<uint8_t>(n, 0));

        for (int r = 0; r < m; r++) {
            dfs(heights, pac, r, 0, m, n);
            dfs(heights, atl, r, n - 1, m, n);
        }
        for (int c = 0; c < n; c++) {
            dfs(heights, pac, 0, c, m, n);
            dfs(heights, atl, m - 1, c, m, n);
        }

        std::vector<std::vector<int>> out;
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (pac[r][c] && atl[r][c]) { out.push_back({r, c}); }
            }
        }
        return out;
    }

private:
    void dfs(std::vector<std::vector<int>>& heights,
             std::vector<std::vector<uint8_t>>& visited,
             int r, int c, int m, int n) {
        if (visited[r][c]) { return; }
        visited[r][c] = 1;

        constexpr int DR[] = {1, -1, 0, 0};
        constexpr int DC[] = {0, 0, -1, 1};
        for (int d = 0; d < 4; d++) {
            int nr = r + DR[d];
            int nc = c + DC[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            if (heights[nr][nc] < heights[r][c]) { continue; } // water flows downhill
            dfs(heights, visited, nr, nc, m, n);
        }
    }
};

// reverse flow, uphill dfs: reverse, start from ocean, flow uphill
