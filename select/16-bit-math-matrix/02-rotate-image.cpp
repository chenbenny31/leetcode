// LC 48 Rotate Image (Medium) — transpose + reverse rows
// Rotate an n x n matrix 90 degrees clockwise, in place.
// Clarify: square only; O(1) extra space required; clockwise; n = 1.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::swap;
using std::reverse;

// Core idea: transpose across the main diagonal, then reverse every row.
// T: O(n^2), S: O(1).
void rotate(vector<vector<int>>& matrix) {
    int n = matrix.size();
    for (int r = 0; r < n; r++) {
        // c starts past the diagonal so each pair swaps once
        for (int c = r + 1; c < n; c++) {
            swap(matrix[r][c], matrix[c][r]);
        }
    }
    for (vector<int>& row : matrix) {
        reverse(row.begin(), row.end());
    }
}

void test(const string& label, vector<vector<int>> matrix,
          const vector<vector<int>>& want) {
    rotate(matrix);
    cout << label << (matrix == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one", {{7}}, {{7}});
    test("two", {{1, 2}, {3, 4}}, {{3, 1}, {4, 2}});
    test("three", {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, {{7, 4, 1}, {8, 5, 2}, {9, 6, 3}});
    return 0;
}

// follow-up (ideas only):
// - m x n, not square -> no in-place swap; build a new n x m matrix where row r becomes
//   column m - 1 - r; O(m * n) space
// - is the matrix symmetric -> must be square (check row sizes first), then compare
//   each cell above the diagonal with its mirror
// - counterclockwise -> transpose, then reverse each column (or the row order) instead
//   of each row
// - one pass -> rotate ring by ring, cycling 4 cells at a time with a temp; each cell
//   (r, c) takes the value from (n - 1 - c, r)
