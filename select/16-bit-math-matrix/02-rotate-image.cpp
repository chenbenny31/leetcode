// LC 48 Rotate Image (Medium). Pattern: transpose + reverse rows.
// Rotate an n x n matrix 90 degrees clockwise, in place.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::swap;
using std::reverse;

// Transpose across the main diagonal, then reverse every row. T: O(n^2), S: O(1).
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

// Follow-up: m x n (not square) -> no in-place swap; write a new n x m matrix where row r
// becomes column m - 1 - r: out[c][m - 1 - r] = mat[r][c]. T: O(m*n), S: O(m*n).
vector<vector<int>> rotateRect(const vector<vector<int>>& mat) {
    int m = mat.size();
    int n = mat[0].size();
    vector<vector<int>> out(n, vector<int>(m));
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            out[c][m - 1 - r] = mat[r][c];
        }
    }
    return out;
}

// J6: symmetric across the main diagonal = square and mat[r][c] == mat[c][r].
// T: O(n^2), S: O(1).
bool isSymmetric(const vector<vector<int>>& mat) {
    int n = mat.size();
    // a non-square matrix is never symmetric (and mat[c][r] could be out of range)
    for (const vector<int>& row : mat) {
        if ((int)row.size() != n) { return false; }
    }
    for (int r = 0; r < n; r++) {
        for (int c = r + 1; c < n; c++) {
            if (mat[r][c] != mat[c][r]) { return false; }
        }
    }
    return true;
}

void test(const string& label, vector<vector<int>> matrix,
          const vector<vector<int>>& want) {
    rotate(matrix);
    cout << label << (matrix == want ? " PASS\n" : " FAIL\n");
}

void testRect(const string& label, const vector<vector<int>>& mat,
              const vector<vector<int>>& want) {
    cout << label << (rotateRect(mat) == want ? " PASS\n" : " FAIL\n");
}

void testSymmetric(const string& label, const vector<vector<int>>& mat, bool want) {
    cout << label << (isSymmetric(mat) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one", {{7}}, {{7}});
    test("two", {{1, 2}, {3, 4}}, {{3, 1}, {4, 2}});
    test("three", {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}, {{7, 4, 1}, {8, 5, 2}, {9, 6, 3}});
    testRect("rect-row", {{1, 2, 3}}, {{1}, {2}, {3}});
    testRect("rect-tall", {{1, 2}, {3, 4}, {5, 6}}, {{5, 3, 1}, {6, 4, 2}});
    testRect("rect-square", {{1, 2}, {3, 4}}, {{3, 1}, {4, 2}});
    testSymmetric("sym-yes", {{1, 7}, {7, 2}}, true);
    testSymmetric("sym-no", {{1, 7}, {8, 2}}, false);
    testSymmetric("sym-non-square", {{1, 2}}, false);
    testSymmetric("sym-single", {{5}}, true);
}

// follow-up: counterclockwise -> transpose, then reverse each column (or reverse the
//   row order) instead of each row
// follow-up: one pass -> rotate ring by ring, cycling 4 cells at a time with a temp:
//   (r, c) <- (n-1-c, r) <- (n-1-r, n-1-c) <- (c, n-1-r) <- (r, c)
