// dfs recursive with in-place marking, T: O(mn), S: O(mn) stack frames worst case

#include <vector>

class Solution {
public:
    int numIslands(std::vector<std::vector<char>>& grid) {
        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());
        int count = 0;

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (grid[r][c] == '1') {
                    dfs(grid, r, c, m, n);
                    count++;
                }
            }
        }
        return count;
    }

private:
    void dfs(std::vector<std::vector<char>>& grid,
             int r, int c, int m, int n) {
        if (r < 0 || r >= m || c < 0 || c >= n) { return; }
        if (grid[r][c] != '1') { return; }

        grid[r][c] = '2';
        dfs(grid, r + 1, c, m, n);
        dfs(grid, r - 1, c, m, n);
        dfs(grid, r, c + 1, m, n);
        dfs(grid, r, c - 1, m, n);
    }
};

// dfs iterative with in-place marking, T: O(mn), S: O(mn) heap

#include <vector>
#include <stack>
#include <utility> // std:pair

class Solution {
public:
    int numIslands(std::vector<std::vector<char>>& grid) {
        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());
        int count = 0;

        std::vector<std::pair<int, int>> buf;
        buf.reserve(m * n);
        std::stack<std::pair<int, int>, std::vector<std::pair<int, int>>> stk(std::move(buf));

        constexpr int DR[] = {1, -1, 0, 0};
        constexpr int DC[] = {0, 0, 1, -1};

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (grid[r][c] != '1') { continue; }
                count++;
                grid[r][c] = '2';
                stk.push({r, c});

                while (!stk.empty()) {
                    auto [cr, cc] = stk.top(); stk.pop();
                    for (int d = 0; d < 4; d++) {
                        int nr = cr + DR[d];
                        int nc = cc + DC[d];
                        if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                        if (grid[nr][nc] != '1') { continue; }
                        grid[nr][nc] = '2';
                        stk.push({nr, nc});
                    }
                }
            }
        }
        return count;
    }
};

// union-find, O(mn*a(mn)), S: O(mn)

#include <vector>
#include <utility> // std::swap

class Solution {
public:
    int numIslands(std::vector<std::vector<char>>& grid) {
        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());

        std::vector<int> parent(m * n);
        std::vector<int> rnk(m * n, 0);
        int count = 0;

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (grid[r][c] == '1') { 
                    parent[r * n + c] = r * n + c;
                    count++;
                } else {
                    parent[r * n + c] = -1;
                }
            }
        }

        auto find = [&](int x) {
            while (parent[x] != x) {
                parent[x] = parent[parent[x]];
                x = parent[x];
            }
            return x;
        };

        auto unite = [&](int a, int b) {
            a = find(a); b = find(b);
            if (a == b) { return; }
            if (rnk[a] < rnk[b]) { std::swap(a, b); }
            parent[b] = a;
            if (rnk[a] == rnk[b]) { rnk[a]++; }
            count--;
        };

        constexpr int DR[] = {1, 0};
        constexpr int DC[] = {0, 1};
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (grid[r][c] == '1') {
                    for (int d = 0; d < 2; d++) {
                        int nr = r + DR[d];
                        int nc = c + DC[d];
                        if (nr < 0 || nr >= m || nc < 0 || nc >= n) { continue; }
                        if (grid[nr][nc] == '1') { unite(r * n + c, nr * n + nc); }
                    }
                }
            }
        }
        return count; 
    }
};

// UF only checks right and down, two direction sufficient, each edge process once
// dfs recursive vs iterative: system stack (24-32 bytes per recursive frame) vs heap-allocated vector (std::pair<int, int> 8 bytes per entry)
