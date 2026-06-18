// multi-source bfs, T: O(mn), S: O(mn)

#include <vector>
#include <utility> // std::pair
#include <climits> // INT_MAX

class Solution {
public:
    std::vector<std::vector<int>> updateMatrix(std::vector<std::vector<int>>& mat) {
        int m = static_cast<int>(mat.size());
        int n = static_cast<int>(mat[0].size());

        std::vector<std::vector<int>> dist(m, std::vector<int>(n, INT_MAX));
        std::vector<std::pair<int, int>> q;
        q.reserve(m * n);
        int head = 0;

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (mat[r][c] == 0) {
                    dist[r][c] = 0;
                    q.push_back({r, c});
                }
            }
        }

        constexpr int DR[] = {1, -1, 0, 0};
        constexpr int DC[] = {0, 0, -1, 1};

        while (head < static_cast<int>(q.size())) {
            auto [r, c] = q[head];
            head++;
            for (int d = 0; d < 4; d++) {
                int nr = r + DR[d];
                int nc = c + DC[d];
                if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                if (dist[nr][nc] != INT_MAX) { continue; }
                dist[nr][nc] = dist[r][c] + 1;
                q.push_back({nr, nc});
            }
        }
        return dist;
    }
};

// dp, T: O(mn), S: O(1)

#include <vector>
#include <algorithm> // std::min
#include <climits> // INT_MAX

class Solution {
public:
    std::vector<std::vector<int>> updateMatrix(std::vector<std::vector<int>>& mat) {
        int m = static_cast<int>(mat.size());
        int n = static_cast<int>(mat[0].size());
        std::vector<std::vector<int>> dist(m, std::vector<int>(n, INT_MAX - 1));

        // pass 1: top-left to bottom-right, propagate from top and left
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (mat[r][c] == 0) { dist[r][c] = 0; continue; }
                if (r > 0 && dist[r - 1][c] != INT_MAX - 1) {
                    dist[r][c] = std::min(dist[r][c], dist[r - 1][c] + 1);
                }
                if (c > 0 && dist[r][c - 1] != INT_MAX - 1) {
                    dist[r][c] = std::min(dist[r][c], dist[r][c - 1] + 1);
                }
            }
        }

        // pass 2: bottom-right to top-left - propagate from bottom and right
        for (int r = m - 1; r >= 0; r--) {
            for (int c = n - 1; c >= 0; c--) {
                if (r + 1 < m && dist[r + 1][c] != INT_MAX - 1) {
                    dist[r][c] = std::min(dist[r][c], dist[r + 1][c] + 1);
                }
                if (c + 1 < n && dist[r][c + 1] != INT_MAX - 1) {
                    dist[r][c] = std::min(dist[r][c], dist[r][c + 1] + 1);
                }
            }
        }

        return dist;
    }
};

// two-pass for dp: cover four neighbors, correct shortest path
// dp vs bfs: two sequentail passes over contiguous memory, better cache behavior than bfs
