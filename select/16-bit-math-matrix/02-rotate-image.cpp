// LC 48 Rotate Image (Medium) — transpose + reverse rows
// rotate an n x n matrix 90 degrees clockwise, in place
//
// clarify:
// square only -> in-place swap works; m x n -> follow-up
// O(1) extra space required -> no copy
// direction -> clockwise
// n = 1 -> unchanged

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::swap;
using std::reverse;

// core idea: transpose across the main diagonal, then reverse every row
// T: O(n^2), S: O(1)
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

// follow-up:
// m x n, not square -> new n x m matrix: row r becomes column m - 1 - r; O(m * n) space
// is the matrix symmetric -> must be square; compare cells above diagonal w/ mirrors
// counterclockwise -> transpose, then reverse each column (or the row order)
// one pass -> ring by ring, cycle 4 cells w/ a temp; (r, c) takes from (n - 1 - c, r)
