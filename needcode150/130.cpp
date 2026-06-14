// dfs with border, T: O(mn), S: O(mn) stack frames

#include <vector>

class Solution {
public:
    void solve(std::vector<std::vector<char>>& board) {
        int m = static_cast<int>(board.size());
        int n = static_cast<int>(board[0].size());

        // mark boarder-connected '0' as safe
        for (int r = 0; r < m; r++) {
            dfs(board, r, 0, m, n);
            dfs(board, r, n - 1, m, n);
        }
        for (int c = 0; c < n; c++) {
            dfs(board, 0, c, m, n);
            dfs(board, m - 1, c, m, n);
        }

        // flip: 'O' to 'X', 'S' to 'O'
        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (board[r][c] == 'O') { board[r][c] = 'X'; }
                if (board[r][c] == 'S') { board[r][c] = 'O'; }
            }
        }
    }

private:
    void dfs(std::vector<std::vector<char>>& board,
             int r, int c, int m, int n) {
        if (r < 0 || r >= m || c < 0 || c >= n) { return; }
        if (board[r][c] != 'O') { return; }
        board[r][c] = 'S'; // safe sentinel
        dfs(board, r + 1, c, m, n);
        dfs(board, r - 1, c, m, n);
        dfs(board, r, c + 1, m, n);
        dfs(board, r, c - 1, m, n);
    }
};

// union-find, T: O(mn * a(mn)), S: O(mn)

#include <vector>
#include <utility> // std::swap

class Solution {
public:
    void solve(std::vector<std::vector<char>>& board) {
        int m = static_cast<int>(board.size());
        int n = static_cast<int>(board[0].size());
        int border = m * n; // virtual boarder node

        std::vector<int> parent(m * n + 1);
        std::vector<int> rnk(m * n + 1, 0);
        for (int i = 0; i <= m * n; i++) { parent[i] = i; }

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
        };

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (board[r][c] != 'O') { continue; }
                int idx = r * n + c;
                if (r == 0 || r == m - 1 || c == 0 || c == n - 1) { unite(idx, border); }
                if (r + 1 < m && board[r + 1][c] == 'O') { unite(idx, (r + 1) * n + c); }
                if (c + 1 < n && board[r][c + 1] == 'O') { unite(idx, r * n + c + 1); }
            }
        }

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                if (board[r][c] == 'O' && find(r * n + c) != find(border)) { board[r][c] = 'X'; }
            }
        }
    }
};

// border-first dfs: dfs to mark all border connected spot safe
// why not dfs from interior 'O': O(mn) dfs per cell, border-first has O(m+n) seeds, O(mn) total marking
