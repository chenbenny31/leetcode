// track rows and cols contain zeros, T: O(mn), S: O(m+n)

#include <vector>
#include <cstdint> // uint8_t

class Solution {
public:
    void setZeroes(std::vector<std::vector<int>>& matrix) {
        int m = static_cast<int>(matrix.size());
        int n = static_cast<int>(matrix[0].size());

        std::vector<uint8_t> zeroRow(m, 0);
        std::vector<uint8_t> zeroCol(n, 0);

        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                if (matrix[r][c] == 0) {
                    zeroRow[r] = 1;
                    zeroCol[c] = 1;
                }
            }
        }

        for (int r = 0; r < m; ++r) {
            for (int c = 0; c < n; ++c) {
                if (zeroRow[r] || zeroCol[c]) { matrix[r][c] = 0; }
            }
        }
    }
};

// track in-place of matrix, T: O(mn), S: O(1)

#include <vector>

class Solution {
public:
    void setZeroes(std::vector<std::vector<int>>& matrix) {
        int m = static_cast<int>(matrix.size());
        int n = static_cast<int>(matrix[0].size());

        bool firstRowZero = false;
        bool firstColZero = false;

        for (int c = 0; c < n; ++c) {
            if (matrix[0][c] == 0) { firstRowZero = true; }
        }
        for (int r = 0; r < m; ++r) {
            if (matrix[r][0] == 0) { firstColZero = true; }
        }

        // use first row/col as markers
        for (int r = 1; r < m; ++r) {
            for (int c = 1; c < n; ++c) {
                if (matrix[r][c] == 0) {
                    matrix[r][0] = 0;
                    matrix[0][c] = 0;
                }
            }
        }

        // zero out marked row and col
        for (int r = 1; r < m; ++r) {
            for (int c = 1; c < n; ++c) {
                if (matrix[r][0] == 0 || matrix[0][c] == 0) { matrix[r][c] = 0; }
            }
        }

        // restore first row and col
        if (firstRowZero) {
            for (int c = 0; c < n; ++c) { matrix[0][c] = 0; }
        }
        if (firstColZero) {
            for (int r = 0; r < m; ++r) { matrix[r][0] = 0; }
        }
    }
};

// check first row and col before marking: preserve first row and col zeroes state in var
