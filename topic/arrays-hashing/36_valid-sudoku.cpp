// fixed uint8_t constraint tables, T: O(1), S: O(1)

#include <vector>
#include <cstdint> // uint8_t

class Solution {
public:
    bool isValidSudoku(std::vector<std::vector<char>>& board) {
        constexpr int SZ = 9;
        constexpr int BOX = 3;
        constexpr char EMPTY = '.';
        constexpr char BASE = '1';

        std::uint8_t row[SZ][SZ] = {}; // row[i][j] = 1 means row_i has num j
        std::uint8_t col[SZ][SZ] = {};
        std::uint8_t box[SZ][SZ] = {};

        for (int r = 0; r < SZ; r++) {
            for (int c = 0; c < SZ; c++) {
                char ch = board[r][c];
                if (ch == EMPTY) { continue; }

                int d = ch - BASE;
                int b = (r / BOX) * BOX + (c / BOX);

                if (row[r][d] || col[c][d] || box[b][d]) { return false; }
                row[r][d] = 1;
                col[c][d] = 1;
                box[b][d] = 1;
            }
        }
        return true;
    }
};
