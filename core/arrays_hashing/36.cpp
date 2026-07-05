// fixed uint8_t constraint tables, T: O91), S: O91)

#include <vector>
#include <cstdint> // uint8_t

class Solution {
private:
    static constexpr int N = 9;
    static constexpr int BOX_SZ = 3;

    int box_idx(int r, int c) const {
        return (r / BOX_SZ) * BOX_SZ + (c / BOX_SZ);
    }

public:
    bool isValidSudoku(std::vector<std::vector<char>>& board) {
        constexpr char EMPTY = '.';
        constexpr char BASE = '1';

        std::uint8_t row_seen[N][N] = {}; // row_seen[r][d] = 1, means row r has num d
        std::uint8_t col_seen[N][N] = {};
        std::uint8_t box_seen[N][N] = {};

        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                char val = board[r][c];
                if (val == EMPTY) { continue; }

                int d = val - BASE;
                int box = box_idx(r, c);

                if (row_seen[r][d] || col_seen[c][d] || box_seen[box][d]) { return false; }

                row_seen[r][d] = 1;
                col_seen[c][d] = 1;
                box_seen[box][d] = 1;
            }
        }
        return true;
    }
};

// mem layout: 9*9*3 contiguous stack bytes, always L1-resident
// branch prediction: early-return is the rare branch, predictor settles into "no duplicate", branch-clean on one fails
// follow-up shrink than 243 bytes: use bitmask, uint16_t per group, bit d set if digit d+1 seen, three uint16_t[9]
