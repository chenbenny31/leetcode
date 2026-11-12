// LC 1277 Count Square Submatrices with All Ones (Medium). Pattern: 2D DP.
// matrix is an m x n grid of 0 / 1 (m, n >= 1). Count every square submatrix, of any
// size, whose cells are all 1.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::min;

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

void test(const string& label, const vector<vector<int>>& matrix, int want) {
    bool ok = countSquares(matrix) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("full-3x3", {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}}, 14);
    test("corner-zero", {{0, 1, 1}, {1, 1, 1}, {1, 1, 1}}, 11);
    test("hole", {{1, 1, 1}, {1, 0, 1}, {1, 1, 1}}, 8);
    test("row", {{1, 0, 1, 1}}, 3);
    test("zeros", {{0, 0}, {0, 0}}, 0);
}

// follow-up: LC 221 Maximal Square -> same dp, answer = (max dp[r][c])^2
// follow-up: straightforward -> each top-left corner, grow the side while the new row and
//   column are all 1; O(m * n * min(m, n)^2)
// follow-up: O(n) space -> keep only the previous dp row (plus the old up-left value)
