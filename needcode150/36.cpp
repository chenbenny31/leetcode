// hash-set + encoding-key, T: O(1), S: O(1)

#include <vector>
#include <unordered_set>
#include <string>

class Solution {
public:
    bool isValidSudoku(std::vector<std::vector<char>>& board) {
        constexpr int N = 9;
        std::unordered_set<std::string> seen;
        seen.reserve(N * N * 3);
        seen.max_load_factor(0.25f);

        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (board[r][c] == '.') { continue; }

                char d = board[r][c];
                int box = (r / 3) * 3 + (c / 3);

                std::string row_key = "r" + std::to_string(r) + d;
                std::string col_key = "c" + std::to_string(c) + d;
                std::string box_key = "b" + std::to_string(box) + d;

                if (!seen.insert(row_key).second) { return false; }
                if (!seen.insert(col_key).second) { return false; }
                if (!seen.insert(box_key).second) { return false; }
            }
        }
        return true;
    }
};

// flat-array, encode seen digits as bitmasks, T: O(1), S: O(1) stack-allocated

#include <vector>
#include <cstring>

class Solution {
public:
    bool isValidSudoku(std::vector<std::vector<char>>& board) {
        constexpr int N = 9;

        int rows[N];
        int cols[N];
        int boxes[N];
        std::memset(rows, 0, sizeof(rows));
        std::memset(cols, 0, sizeof(cols));
        std::memset(boxes, 0, sizeof(boxes));

        for (int r = 0; r < N; r++) {
            for (int c = 0; c < N; c++) {
                if (board[r][c] == '.') { continue; }

                int d = board[r][c] - '1'; // 0-indexed
                int mask = 1 << d;
                int box = (r / 3) * 3 + (c / 3);

                if (rows[r] & mask) { return false; }
                if (cols[c] & mask) { return false; }
                if (boxes[box] & mask) { return false; }

                rows[r] |= mask;
                cols[c] |= mask;
                boxes[box] |= mask;
            }
        }
        return true;
    }
};

// three arrays, 108 bytes total, fits in two cache lines, L1 resident, all single index laod and bitwise ops, no heap
// bitmask op: mask & seen for test dup, seen |= mask for set
// validate and solve: ~(rows[r] | cols[c] | boxes[box]) & 0x1FF
// hash-set vs bitmask: bitmask is strictly better for fixex-size
