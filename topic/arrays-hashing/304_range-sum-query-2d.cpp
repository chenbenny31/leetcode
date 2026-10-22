// prefix, T: O(mn), S: O(mn)

#include <vector>

class NumMatrix {
private:
    std::vector<std::vector<int>> prefix; // prefix[r][c] = sum of matrix[0..r)[0..c), prefix[0][0] = 0

public:
    NumMatrix(std::vector<std::vector<int>>& matrix) {
        const int m = static_cast<int>(matrix.size());
        const int n = m ? static_cast<int>(matrix[0].size()) : 0;
        prefix.assign(m + 1, std::vector<int>(n + 1, 0));

        for (int r = 0; r < m; r++) {
            for (int c = 0; c < n; c++) {
                prefix[r + 1][c + 1] = matrix[r][c]
                                     + prefix[r][c + 1] + prefix[r + 1][c]
                                     - prefix[r][c]; // overlap
            }
        }
    }

    int sumRegion(int row1, int col1, int row2, int col2) {
        return prefix[row2 + 1][col2 + 1]
               - prefix[row1][col2 + 1] - prefix[row2 + 1][col1]
               + prefix[row1][col1]; // overlap
    }
};
