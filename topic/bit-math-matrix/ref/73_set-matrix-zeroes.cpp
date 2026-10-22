// LC 73 Set Matrix Zeroes: if an element of an m x n matrix is 0, set its whole row and
// column to 0, in place.
// Follow-up: use O(1) extra space.
#include <iostream>
#include <string>
#include <vector>
using std::cout;
using std::string;
using std::vector;

// Record zero rows and columns first, then clear; clearing while scanning would
// spread new zeros. T: O(m*n), S: O(m+n).
void setZeroes(vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    vector<bool> zeroRow(m, false);   // zeroRow[r] = row r contains a 0
    vector<bool> zeroCol(n, false);   // zeroCol[c] = column c contains a 0
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (matrix[r][c] == 0) {
                zeroRow[r] = true;
                zeroCol[c] = true;
            }
        }
    }
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (zeroRow[r] || zeroCol[c]) { matrix[r][c] = 0; }
        }
    }
}

// Follow-up: row 0 and column 0 double as the flags for the other rows and columns;
// their own zero status is saved first. T: O(m*n), S: O(1).
void setZeroesConstSpace(vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    bool row0Zero = false;
    bool col0Zero = false;
    for (int c = 0; c < n; c++) {
        if (matrix[0][c] == 0) { row0Zero = true; }
    }
    for (int r = 0; r < m; r++) {
        if (matrix[r][0] == 0) { col0Zero = true; }
    }
    for (int r = 1; r < m; r++) {
        for (int c = 1; c < n; c++) {
            if (matrix[r][c] == 0) {
                matrix[r][0] = 0;
                matrix[0][c] = 0;
            }
        }
    }
    for (int r = 1; r < m; r++) {
        for (int c = 1; c < n; c++) {
            if (matrix[r][0] == 0 || matrix[0][c] == 0) { matrix[r][c] = 0; }
        }
    }
    if (row0Zero) {
        for (int c = 0; c < n; c++) { matrix[0][c] = 0; }
    }
    if (col0Zero) {
        for (int r = 0; r < m; r++) { matrix[r][0] = 0; }
    }
}

void test(const string& label, vector<vector<int>> matrix,
          const vector<vector<int>>& want) {
    setZeroes(matrix);
    cout << label << (matrix == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("middle", {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}, {{1, 0, 1}, {0, 0, 0}, {1, 0, 1}});
    test("corner", {{0, 1}, {1, 1}}, {{0, 0}, {0, 1}});
    test("no-zero", {{1, 2}, {3, 4}}, {{1, 2}, {3, 4}});
}
