// LC 1277 Count Square Submatrices with All Ones (Medium). Pattern: 2D DP.
// (1) m x n grid of 0 / 1: cnt all-ones square submatrices of any size
// (2) LC 221 Maximal Square (reported with it): area of the largest all-ones square
//
// clarify:
// ints 0 / 1 or chars '0' / '1' -> ints here
// overlapping squares -> all count
// m, n -> >= 1, up to 300
// return cnt, side or area -> (1) cnt, (2) area

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;
using std::max;

// (1) count squares
// core idea: dp over bottom-right corners, side = 1 + min(up, left, up-left)
// cell w/ side s ends exactly s squares (sides 1..s) -> answer = sum of dp
// T: O(m * n), S: O(m * n)
int countSquares(const vector<vector<int>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    // dp[r][c] = side of largest all-ones square w/ bottom-right corner (r, c)
    vector<vector<int>> dp(m, vector<int>(n, 0));
    int total = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            if (matrix[r][c] == 0) { continue; }
            if (r == 0 || c == 0) {
                dp[r][c] = 1;
            } else {
                dp[r][c] = 1 + min(dp[r - 1][c], min(dp[r][c - 1], dp[r - 1][c - 1]));
            }
            total += dp[r][c];
        }
    }
    return total;
}

// (2) maximal square
// core idea: same recurrence, max instead of sum, kept in one row
// row[c] = "up" until overwritten, row[c - 1] = "left", diag = old row[c - 1]
// T: O(m * n), S: O(n)
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
            // 0 cell must be written too: row[c] still holds the row above
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
    test("row", {{1, 0, 1, 1}}, 3);
    test("zeros", {{0, 0}, {0, 0}}, 0);
    testMax("max-full-3x3", {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}, 9);
    testMax("max-corner-zero", {{0, 1, 1}, {1, 1, 1}, {1, 1, 1}}, 4);
    testMax("max-stale-zero", {{1, 1}, {1, 0}, {1, 1}}, 1);
    testMax("max-zeros", {{0, 0}, {0, 0}}, 0);
}

// follow-up:
// brute force first -> grow square from each top-left corner, O(m * n * min(m, n)^2)
// O(n) space for the cnt -> one-row trick of maximalSquare, sum row[c]
// where is the largest square -> max grows at (r, c): top-left = (r-side+1, c-side+1)
// largest all-ones rectangle (LC 85) -> per row, column heights of ones = histogram
// histogram code -> select/optional/05-stack/04-largest-rectangle-histogram, once per row
