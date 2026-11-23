// 08 · Row Max / Column Min (Cisco OA, Easy) — row maxes + column mins
// N x M matrix of non-negative ints; return a value that is the max of its row and the
// min of its column, -1 if there is none (the most reported Cisco OA problem)
//
// clarify:
// which way round -> Cisco: max in its row, min in its column (LC 1380 is the reverse)
// several answers -> only when values repeat; return the first one in row-major order
// empty matrix -> not given (N, M >= 1)
// output -> the value, -1 if none

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

// core idea: one pass for every row max and column min, a second pass to test each cell
// T: O(R * C), S: O(R + C)
int rowMaxColMin(const vector<vector<int>>& matrix) {
    int rows = matrix.size();
    int cols = matrix[0].size();
    vector<int> rowMax(rows, INT_MIN);
    vector<int> colMin(cols, INT_MAX);
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            rowMax[r] = max(rowMax[r], matrix[r][c]);
            colMin[c] = min(colMin[c], matrix[r][c]);
        }
    }
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            if (matrix[r][c] == rowMax[r] && matrix[r][c] == colMin[c]) {
                return matrix[r][c];
            }
        }
    }
    return -1;
}

void test(const string& label, const vector<vector<int>>& matrix, int want) {
    cout << label << (rowMaxColMin(matrix) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("found", {{1, 2}, {3, 4}}, 2);
    test("none", {{1, 4}, {3, 2}}, -1);
    test("single-row", {{7, 8}}, 8);
    test("single-col", {{5}, {3}}, 3);
    return 0;
}

// follow-up:
// 1. LC 1380 lucky nums (row min, column max) -> swap max / min in the first pass
// 2. distinct values -> at most one answer: min of row maxes == max of column mins
// 3. O(1) extra space -> for each row find its max col, then scan that column once
// 4. diagonal zig-zag traverse (LC 498, Cisco OA) -> diagonal d = r + c; reverse even d
// 5. rotate 90 degrees (LC 48, Cisco OA) -> select/16-bit-math-matrix/02-rotate-image
