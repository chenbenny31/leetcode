// backtracking dfs, T: O(nm4^l), S: O(l) recursion path

#include <vector>
#include <string>

class Solution {
public:
    bool exist(std::vector<std::vector<char>>& board, std::string word) {
        int m = static_cast<int>(board.size());
        int n = static_cast<int>(board[0].size());

        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                if (dfs(board, word, r, c, 0, m, n)) { return true; }
            }
        }
        return false;
    }

private:
    bool dfs(std::vector<std::vector<char>>& board, const std::string word,
             int r, int c, int idx, int m, int n) {
        if (idx == static_cast<int>(word.size())) { return true; }
        if (r < 0 || r >= m || c < 0 || c >= n) { return false; }
        if (board[r][c] != word[idx]) { return false; }
        if (board[r][c] == '#') { return false; }

        char orig = board[r][c];
        board[r][c] = '#'; // mark visited

        bool found = dfs(board, word, r + 1, c, idx + 1, m, n) ||
                     dfs(board, word, r - 1, c, idx + 1, m, n) ||
                     dfs(board, word, r, c + 1, idx + 1, m, n) ||
                     dfs(board, word, r, c - 1, idx + 1, m, n);

        board[r][c] = orig; // restore
        return found;
    }
};

// check order in dfs: idx == word.size() first, based case before bounds check, character mismatch over marker duplicate since it's more common
// frequency pruning: O(mn + L) preprocessing, if word needs more character than board contains prune before dfs
// rare-end heuristic: start search from rarer characters: fewer starting cells and less dfs branching
