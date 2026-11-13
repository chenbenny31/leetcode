// LC 54 Spiral Matrix (Medium) — four shrinking bounds
// Return all elements of a non-empty m x n matrix in clockwise spiral order, starting
// at the top-left.
// Clarify: empty matrix; non-square shapes (one row, one column); clockwise from the
//   top-left.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Core idea: keep the bounds of the rectangle not yet visited and peel one ring per
// lap, shrinking a bound after each side. T: O(m * n), S: O(1) besides the output.
vector<int> spiralOrder(const vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    int top = 0;
    int bottom = m - 1;
    int left = 0;
    int right = n - 1;
    vector<int> result;
    while (top <= bottom && left <= right) {
        for (int c = left; c <= right; c++) {
            result.push_back(matrix[top][c]);
        }
        top++;

        for (int r = top; r <= bottom; r++) {
            result.push_back(matrix[r][right]);
        }
        right--;

        // a single remaining row was already read left to right
        if (top <= bottom) {
            for (int c = right; c >= left; c--) {
                result.push_back(matrix[bottom][c]);
            }
            bottom--;
        }

        // a single remaining column was already read top to bottom
        if (left <= right) {
            for (int r = bottom; r >= top; r--) {
                result.push_back(matrix[r][left]);
            }
            left++;
        }
    }
    return result;
}

void test(const string& label, const vector<vector<int>>& matrix,
          const vector<int>& want) {
    cout << label << (spiralOrder(matrix) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-row", {{1, 2, 3}}, {1, 2, 3});
    test("one-col", {{1}, {2}, {3}}, {1, 2, 3});
    test("tall", {{1, 2}, {3, 4}, {5, 6}, {7, 8}}, {1, 2, 4, 6, 8, 7, 5, 3});
    test("wide-center-row", {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}},
         {1, 2, 3, 4, 8, 12, 11, 10, 9, 5, 6, 7});
    return 0;
}

// follow-up (ideas only):
// - spiral matrix II (LC 59) -> same four bounds, write an increasing value instead of
//   reading
// - direction-array version -> row / column deltas + visited grid, turn right when the
//   next cell is out of range or visited; O(m * n) extra space
// - set matrix zeroes (LC 73) -> use the first row / column as markers, with two flags
//   for whether the first row / column themselves had a zero
// - game of life (LC 289) -> keep the old state in bit 0 and write the new one to bit
//   1, then shift every cell right by one: in place
