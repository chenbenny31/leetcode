// multi-source bfs, T: O(mn), S: O(mn)

#include <vector>
#include <utility> // std::pair
#include <climits> // INT_MAX

class Solution {
public:
    void wallsAndGates(std::vector<std::vector<int>>& rooms) {
        int m = static_cast<int>(rooms.size());
        int n = static_cast<int>(rooms[0].size());

        std::vector<std::pair<int, int>> q;
        q.reserve(m * n);
        int head = 0;

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (rooms[r][c] == 0) { q.push_back({r, c}); }
            }
        }

        constexpr int DR[] = {1, -1, 0, 0};
        constexpr int DC[] = {0, 0, 1, -1};

        while (head < static_cast<int>(q.size())) {
            auto [r, c] = q[head];
            head++;
            for (int d = 0; d < 4; d++) {
                int nr = r + DR[d];
                int nc = c + DC[d];
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                if (rooms[nr][nc] != INT_MAX) { continue; }
                rooms[nr][nc] = rooms[r][c] + 1;
                q.push_back({nr, nc});
            }
        }
    }
};

// multi-source bfs: all gates need seeded simultaneously, bfs guarantees first-time min path
