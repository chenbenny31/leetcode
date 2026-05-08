// dfs-mark, S: O(mn) recursion depth worst case

#include <vector>

class Solution {
public:
    void solve(std::vector<std::vector<char>>& board) {
        int m = static_cast<int>(board.size());
        int n = static_cast<int>(board[0].size());

        // mark border-connected 'O' as safe
        for (int r = 0; r < m; ++r) {
            dfs(board, r, 0, m, n);
            dfs(board, r, n - 1, m, n);
        }
        for (int c = 0; c < n; ++c) {
            dfs(board, 0, c, m, n);
            dfs(board, m - 1, c, m, n);
        }

        // flip back: 'O' -> 'X', '#' -> 'O'
        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                if (board[r][c] == 'O') { board[r][c] = 'X'; }
                else if (board[r][c] == '#') { board[r][c] = 'O'; }
            }
        }
    }

private:
    void dfs(std::vector<std::vector<char>>& board,
             int r, int c, int m, int n) {
        if (r < 0 || r >= m || c < 0 || c >= n) { return; }
        if (board[r][c] != 'O') { return; }

        board[r][c] = '#';
        dfs(board, r + 1, c, m, n);
        dfs(board, r - 1, c, m, n);
        dfs(board, r, c + 1, m, n);
        dfs(board, r, c - 1, m, n);
    }
};

// union-find, T: O(mn*a(mn)), S: O(mn)

#include <vector>
#include <numeric> // std::iota
#include <utility> // std::swap

class Solution {
private:
    std::vector<int> parent, rnk;

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
        if (rnk[a] == rnk[b]) { rnk[a]++; }
    }

public:
    void solve(std::vector<std::vector<char>>& board) {
        int m = static_cast<int>(board.size());
        int n = static_cast<int>(board[0].size());
        int total = m * n;

        parent.resize(total + 1);
        rnk.resize(total + 1, 0);
        std::iota(parent.begin(), parent.end(), 0);
        int border = total;

        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                if (board[r][c] != 'O') { continue; }
                int idx = r * n + c;
                if (r == 0 || r == m - 1 || c == 0 || c == n - 1) {
                    unite(idx, border);
                }
                if (r + 1 < m && board[r + 1][c] == 'O') { unite(idx, (r + 1) * n + c); }
                if (c + 1 < n && board[r][c + 1] == 'O') { unite(idx, r * n + c + 1); }
            }
        }

        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                if (board[r][c] == 'O' && find(r * n + c) != find(border)) {
                    board[r][c] = 'X';
                }
            }
        }
    }
};

// virtual border node with sentinel total: unite with all border connected 'O'
// dfs stack depth worst case: entire border is 'O';
// union-find one-pass vs dfs-mark two-pass
// union-find win: board is dynamically updated
