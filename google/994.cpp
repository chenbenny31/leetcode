// multi-src bfs, T: O(mn), S: O(mn)

#include <vector>
#include <queue>

class Solution {
public:
    int orangesRotting(std::vector<std::vector<int>>& grid) {
        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());

        std::queue<std::pair<int, int>> q;
        int fresh = 0;

        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                if (grid[r][c] == 2) { q.push({r, c}); }
                if (grid[r][c] == 1) { ++fresh; }
            }
        }

        if (fresh == 0) { return 0; }

        constexpr int DR[] = {-1, 1, 0, 0};
        constexpr int DC[] = {0, 0, -1, 1};
        int minutes = 0;

        while (!q.empty() && fresh > 0) {
            ++minutes;
            int sz = static_cast<int>(q.size());
            for (int i = 0; i < sz; ++i) {
                auto [r, c] = q.front();
                q.pop();

                for (int d = 0; d < 4; ++d) {
                    int nr = r + DR[d];
                    int nc = c + DC[d];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                    if (grid[nr][nc] != 1) { continue; }

                    grid[nr][nc] = 2;
                    --fresh;
                    q.push({nr, nc});
                }
            }
        }
        return fresh == 0 ? minutes : -1;
    }
};

// multi-src bfs: dfs level 0 for simultaneously spreading
// while (!q.empty() && fresh > 0): early stop when all rotted
// in-place marking
// bfs processes by level: spatially local for small grid, std::queue use std::deque (segmented heap) -> flat circular buffer
