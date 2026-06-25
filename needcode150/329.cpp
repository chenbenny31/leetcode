// dfs + memoization, T: O(mn), S: O(mn)

#include <vector>
#include <algorithm> // std::max

class Solution {
public:
    int longestIncreasingPath(std::vector<std::vector<int>>& matrix) {
        int m = static_cast<int>(matrix.size());
        int n = static_cast<int>(matrix[0].size());
        std::vector<std::vector<int>> memo(m, std::vector<int>(n, 0));
        int best = 1;

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                best = std::max(best, dfs(matrix, memo, r, c, m, n));
            }
        }
        return best;
    }

private:
    int dfs(std::vector<std::vector<int>>& matrix,
            std::vector<std::vector<int>>& memo,
            int r, int c, int m, int n) {
        if (memo[r][c]) { return memo[r][c]; }

        constexpr int DR[] = {-1, 1, 0, 0};
        constexpr int DC[] = {0, 0, -1, 1};
        int best = 1;

        for (int d = 0; d < 4; d++) {
            int nr = r + DR[d];
            int nc = c + DC[d];
            if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
            if (matrix[nr][nc] <= matrix[r][c]) { continue; }
            best = std::max(best, 1 + dfs(matrix, memo, nr, nc, m, n));
        }
        return memo[r][c] = best;
    }
};

// topological sort, T: O(mn), S: O(mn)

#include <vector>
#include <utility> // std::pair

class Solution {
public:
    int longestIncreasingPath(std::vector<std::vector<int>>& matrix) {
        int m = static_cast<int>(matrix.size());
        int n = static_cast<int>(matrix[0].size());

        constexpr int DR[] = {-1, 1, 0, 0};
        constexpr int DC[] = {0, 0, -1, 1};

        std::vector<std::vector<int>> indegree(m, std::vector<int>(n, 0));

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                for (int d = 0; d < 4; d++) {
                    int nr = r + DR[d];
                    int nc = c + DC[d];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                    if (matrix[nr][nc] > matrix[r][c]) { indegree[nr][nc]++; }
                }
            }
        }

        std::vector<std::pair<int, int>> q;
        q.reserve(m * n);
        int head = 0;

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (indegree[r][c] == 0) { q.push_back({r, c}); }
            }
        }

        int steps = 0;
        while (head < static_cast<int>(q.size())) {
            int sz = static_cast<int>(q.size()) - head;
            steps++;
            for (int i = 0; i < sz; i++) {
                auto [r, c] = q[head];
                head++;
                for (int d = 0; d < 4; d++) {
                    int nr = r + DR[d];
                    int nc = c + DC[d];
                    if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                    if (matrix[nr][nc] <= matrix[r][c]) { continue; }
                    indegree[nr][nc]--;
                    if (indegree[nr][nc] == 0) { q.push_back({nr, nc}); }
                }
            }
        }
        return steps;
    }
};

// no visited array in dfs: strictly increasing path means no cycle
// memo[r][c] = best as return: assignment return assigned value
