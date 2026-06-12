// bool-array, T: O(9^m), m = empty cells, S: O(1)

#include <vector>
#include <cstring> // std::memset
#include <cstdint> // uint8_t

class Solution {
public:
    void solveSudoku(std::vector<std::vector<char>>& board) {
        constexpr int N = 9;
        uint8_t rows[N][N];
        uint8_t cols[N][N];
        uint8_t boxes[N][N];
        std::memset(rows, 0, sizeof(rows));
        std::memset(cols, 0, sizeof(cols));
        std::memset(boxes, 0, sizeof(boxes));

        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (board[r][c] == '.') { continue; }
                int d = board[r][c] - '1';
                int box = (r / 3) * 3 + (c / 3);
                rows[r][d] = 1;
                cols[c][d] = 1;
                boxes[box][d] = 1;
            }
        }
        solve(board, rows, cols, boxes, 0, 0);
    }

private:
    bool solve(std::vector<std::vector<char>>& board,
               uint8_t rows[][9], uint8_t cols[][9],
               uint8_t boxes[][9], int r, int c) {
        constexpr int N = 9;
        if (r == N) { return true; }

        int nr = (c == N - 1) ? r + 1 : r;
        int nc = (c == N - 1) ? 0 : c + 1;

        if (board[r][c] != '.') { return solve(board, rows, cols, boxes, nr, nc); }

        int box = (r / 3 ) * 3 + (c / 3);
        for (int d = 0; d < N; d++) {
            if (rows[r][d] || cols[c][d] || boxes[box][d]) { continue; }
            board[r][c] = d + '1';
            rows[r][d] = 1;
            cols[c][d] = 1;
            boxes[box][d] = 1;
            if (solve(board, rows, cols, boxes, nr, nc)) { return true; }
            board[r][c] = '.';
            rows[r][d] = 0;
            cols[c][d] = 0;
            boxes[box][d] = 0;
        }
        return false;
    }
};

// cache 3 * uint8_t[9][9] 243 byte each, fits in 4 cache lines as L1 resident, board 81 chars - two cache lines
// constraint check: three indexed loads: rows[r][d] || cols[c][d] || boxes[box][d], short-circuit on first true, no hash-computation and pointer chasing
// box cached per cell: (r/3)*3 + (c/3) computed once before digit loop
// Bitmask follow-up: pack each row/col/box into single int, 9-bit mask, used = rows[r] | cols[c] | boxes[box], avail = (~used) & 0x1FF
// MRV heuristic: choose cell with fewest available digits first, reduce branching factor dramatically, O(81) scan per step to find min
