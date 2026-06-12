// backtracking with bitmask constraints, T: O(n!), S: O(n)

#include <vector>
#include <string>
#include <cstring> // std::memset

class Solution {
public:
    std::vector<std::vector<std::string>> solveNQueens(int n) {
        std::vector<std::vector<std::string>> out;
        std::vector<int> queens(n, -1); // queens[r] = col of queen in row r

        std::vector<uint8_t> cols(n, 0);
        std::vector<uint8_t> diag1(2 * n - 1, 0); // top-left to bottom-right
        std::vector<uint8_t> diag2(2 * n - 1, 0); // top-right to bottom-left

        backtrack(n, 0, queens, cols, diag1, diag2, out);
        return out;
    }

private:
    void backtrack(int n, int row, std::vector<int>& queens, std::vector<uint8_t>& cols,
                   std::vector<uint8_t> diag1, std::vector<uint8_t> diag2,
                   std::vector<std::vector<std::string>>& out) {
        if (row == n) { out.push_back(build(queens, n)); return; }

        for (int c = 0; c < n; c++) {
            if (cols[c] || diag1[row - c + n -1] || diag2[row + c]) { continue; }
            queens[row] = c;
            cols[c] = 1;
            diag1[row - c + n - 1] = 1;
            diag2[row + c] = 1;
            backtrack(n, row + 1, queens, cols, diag1, diag2, out);
            queens[row] = -1;
            cols[c] = 0;
            diag1[row - c + n - 1] = 0;
            diag2[row + c] = 0;
        }
    }

    std::vector<std::string> build(std::vector<int>& queens, int n) {
        std::vector<std::string> board(n, std::string(n, '.'));
        for (int r = 0; r < n; r++) { board[r][queens[r]] = 'Q'; }
        return board;
    }
};

// diagonal indexing: diag1[row-c+n-1], row-c constant on same diagonal, offset by +n-1 to prevent negative, diag2[row+c] range [0, 2n-2]
// uint8_t over vector<bool>: real byte per element, n + (2n-1) * 2 = 5n-2 bytes, fits L1
// queens[r] = col not 2D board: single int per row, O(n) space, build() reconstructs at solution, each row contains at most one queen
