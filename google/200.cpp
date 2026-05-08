// dfs-mark, T: O(mn), S: O(mn) recursion depth worst case

#include <vector>

class Solution {
public:
    int numIslands(std::vector<std::vector<char>>& grid) {
        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());
        int count_ = 0;

        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                if (grid[r][c] == '1') {
                    dfs(grid, r, c, m, n);
                    ++count_;
                }
            }
        }
        // restore grid[][]
        return count_;
    }

private:
    void dfs(std::vector<std::vector<char>>& grid,
             int r, int c, int m, int n) {
        if (r >= m || r < 0 || c >= n || c < 0) { return; }
        if (grid[r][c] != '1') { return; }

        grid[r][c] = '#';
        dfs(grid, r + 1, c, m, n);
        dfs(grid, r - 1, c, m, n);
        dfs(grid, r, c + 1, m, n);
        dfs(grid, r, c - 1, m, n);
    }
};

// union-find, T: O(mn*a(mn)), S: O(mn)

#include <vector>
#include <numeric> // std::iota
#include <utility> // std::swap

class Solution {
private:
    std::vector<int> parent, rnk;
    int components;

    int find(int x) {
        if (parent[x] != x) { parent[x] = find(parent[x]); }
        return parent[x];
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) { return; }

        if (rnk[a] < rnk[b]) { std::swap(a, b); }
        parent[b] = a;
        if (rnk[a] == rnk[b]) { ++rnk[a]; }
        --components;
    }

public:
    int numIslands(std::vector<std::vector<char>>& grid) {
        int m = static_cast<int>(grid.size());
        int n = static_cast<int>(grid[0].size());
        components = 0;
        parent.resize(m * n);
        std::iota(parent.begin(), parent.end(), 0);
        rnk.resize(m * n, 0);

        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                if (grid[r][c] != '1') { continue; }

                ++components;
                if (r + 1 < m && grid[r + 1][c] == '1') { unite(r * n + c, (r + 1) * n + c); }
                if (c + 1 < n && grid[r][c + 1] == '1') { unite(r * n + c, r * n + c + 1); }
            }
        }
        return components;
    }
};
