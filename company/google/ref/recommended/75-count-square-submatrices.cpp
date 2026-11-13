// LC 1277 Count Square Submatrices with All Ones (Medium). Pattern: 2D DP.
// matrix is an m x n grid of 0 / 1 (m, n >= 1). Count every square submatrix, of any
// size, whose cells are all 1.
//
// Clarify:
// - input -> m x n ints 0 / 1, m, n >= 1 (LC 221 gives chars '0' / '1')
// - squares of every size count, overlapping ones too; a single 1 is a 1 x 1 square
// - m, n up to 300 -> O(m * n) DP; the answer is at most ~9e6 (all ones), fits int
// - return the count (LC 221 returns the area of the largest square)
// Idea: dp[r][c] = side of the largest all-ones square with bottom-right corner (r, c)
//   = 1 + min(up, left, up-left): a square of side s there needs side s - 1 at all
//   three. That cell is the corner of exactly dp[r][c] squares (sides 1..s), so the
//   answer is the sum of dp. One pass, each cell O(1).
// Pitfalls:
// - first row / column: dp = 1 on a 1 cell; never read r - 1 or c - 1 there
// - the min of all three; up and left alone miss a hole on the diagonal
// - a 0 cell keeps dp 0: skipping it is fine only because dp starts at 0 (not in a
//   reused row, see maximalSquare)
// - LC 221 returns side * side, not the side
// Tests to write: full 3 x 3 -> 14; a corner zero; a center hole; a single row; all
//   zeros; for the max, a 0 under a 1 that a reused row must not remember.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

// dp over bottom-right corners; a cell with side s ends exactly s squares (sides 1..s),
// so the answer is the sum of dp. T: O(m * n), S: O(m * n).
int countSquares(const vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    // dp[r][c] = side of the largest all-ones square with bottom-right corner (r, c)
    vector<vector<int>> dp(m, vector<int>(n, 0));
    int total = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (matrix[r][c] == 0) { continue; }
            if (r == 0 || c == 0) {
                dp[r][c] = 1;
            } else {
                // the square is capped by the shortest of up, left and up-left
                dp[r][c] = 1 + min(dp[r - 1][c], min(dp[r][c - 1], dp[r - 1][c - 1]));
            }
            total += dp[r][c];
        }
    }
    return total;
}

// follow-up LC 221 Maximal Square (reported with LC 1277): area of the largest
// all-ones square. Same recurrence with max instead of sum, kept in one row: row[c]
// still holds "up" until it is overwritten, row[c - 1] already holds "left", and diag
// keeps the old row[c - 1]. T: O(m * n), S: O(n).
// Pitfall: a 0 cell must write row[c] = 0 and still pass diag on; a `continue` would
// leave the row above's value in row[c].
int maximalSquare(const vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    // row[c] = dp[r][c] once written in this row, dp[r - 1][c] before that
    vector<int> row(n, 0);
    int best = 0;
    for (int r = 0; r < m; r++) {
        // dp[r - 1][c - 1] for the cell being computed
        int diag = 0;
        for (int c = 0; c < n; c++) {
            int up = row[c];
            if (matrix[r][c] == 0) {
                row[c] = 0;
            } else if (r == 0 || c == 0) {
                row[c] = 1;
            } else {
                row[c] = 1 + min(up, min(row[c - 1], diag));
            }
            best = max(best, row[c]);
            diag = up;
        }
    }
    return best * best;
}

void test(const string& label, const vector<vector<int>>& matrix, int want) {
    bool ok = countSquares(matrix) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testMax(const string& label, const vector<vector<int>>& matrix, int want) {
    cout << label << (maximalSquare(matrix) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("full-3x3", {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}, 14);
    test("corner-zero", {{0, 1, 1}, {1, 1, 1}, {1, 1, 1}}, 11);
    test("hole", {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}, 8);
    test("row", {{1, 0, 1, 1}}, 3);
    test("zeros", {{0, 0}, {0, 0}}, 0);
    testMax("max-full-3x3", {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}, 9);
    testMax("max-corner-zero", {{0, 1, 1}, {1, 1, 1}, {1, 1, 1}}, 4);
    testMax("max-hole", {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}, 1);
    testMax("max-stale-zero", {{1, 1}, {1, 0}, {1, 1}}, 1);
    testMax("max-zeros", {{0, 0}, {0, 0}}, 0);
}

// follow-up: LC 221 Maximal Square -> maximalSquare above
// follow-up: straightforward first -> grow a square from each top-left corner (r, c)
//   while the new bottom row and right column are all ones; O(m * n * min(m, n)^2):
//     for (int s = 1; r + s <= m && c + s <= n; s++) {
//         bool full = true;
//         for (int k = 0; k < s; k++) {
//             full = full && matrix[r + s - 1][c + k] && matrix[r + k][c + s - 1];
//         }
//         if (!full) { break; }
//         total++;
//     }
// follow-up: O(n) space -> maximalSquare above keeps one row plus the old up-left value
// follow-up: where is the largest square -> keep its corner when the max grows:
//     if (dp[r][c] > best) { best = dp[r][c]; top = r - best + 1; left = c - best + 1; }
// follow-up: largest all-ones rectangle (LC 85) -> per row, column heights of ones as a
//   histogram, then select/optional/05-stack/04-largest-rectangle-histogram per row
