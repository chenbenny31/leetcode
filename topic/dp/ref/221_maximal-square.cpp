// LC 221 Maximal Square: return the area of the largest square of '1's in a '0'/'1'
// matrix.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::max;
using std::min;
using std::string;
using std::vector;

// 2D DP: a '1' cell's square side = 1 + min(up, left, up-left). T: O(m*n), S: O(m*n).
int maximalSquare(const vector<vector<char>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    // dp[i][j] = side of the largest all-'1' square whose bottom-right corner is
    // matrix[i-1][j-1]; row 0 and column 0 are zero padding
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    int best = 0;

    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (matrix[i - 1][j - 1] == '1') {
                dp[i][j] = 1 + min({dp[i - 1][j], dp[i][j - 1], dp[i - 1][j - 1]});
                best = max(best, dp[i][j]);
            }
        }
    }

    return best * best;
}

void test(const string& label, const vector<vector<char>>& matrix, int want) {
    cout << label << (maximalSquare(matrix) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("corner-square", {{'1', '1', '0'}, {'1', '1', '1'}, {'1', '1', '1'}}, 4);
    test("diagonal", {{'1', '0'}, {'0', '1'}}, 1);
    test("all-zero", {{'0'}}, 0);
}
