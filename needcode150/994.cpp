// multi-source bfs, T: O(mn), S: O(mn)

#include <vector>
#include <utility> // std::pair

class Solution {
public:
    int orangesRotting(std::vector<std::vector<int>>& grid) {
        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());

        std::vector<std::pair<int, int>> q;
        q.reserve(m * n);
        int fresh = 0;
        int head = 0;

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (grid[r][c] == 2) { q.push_back({r, c}); }
                if (grid[r][c] == 1) { fresh++; }
            }
        }

        constexpr int DR[] = {1, -1, 0, 0};
        constexpr int DC[] = {0, 0, -1, 1};
        int minutes = 0;

        while (head < static_cast<int>(q.size()) && fresh > 0) {
            int sz = static_cast<int>(q.size()) - head;
            minutes++;
            for (int i = 0; i < sz; i++) {
                auto [r, c] = q[head];
                head++;
                for (int d = 0; d < 4; d++) {
                    int nr = r + DR[d];
                    int nc = c + DC[d];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                    if (grid[nr][nc] != 1) { continue; }
                    grid[nr][nc] = 2;
                    fresh--;
                    q.push_back({nr, nc});
                }
            }
        }
        return fresh == 0 ? minutes : -1;
    }
};

// multi-source bfs: all seeded simultanously
// std::vector as queue with head index: contigous memory, pre-reserve, sz = q.size()-head gives current level size
// while (head < q.size() && fresh > 0): two termination conditions, bfs exhausted or all fresh rotted
// fresh count: early exit if fresh == 0
