// LC 221 Maximal Square: matrix of '0'/'1' characters. Return the area of the largest
// square containing only '1'.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::max;
using std::min;
using std::string;
using std::vector;

// dp[i][j] = 1 + min(up, left, up-left), compressed to one row with a saved diagonal.
// T: O(m*n), S: O(n).
int maximalSquare(const vector<vector<char>>& matrix) {
    int m = matrix.size();
    int n = matrix[0].size();
    // dp[j+1] = side of the largest all-'1' square whose bottom-right corner is (i, j)
    vector<int> dp(n + 1, 0);
    int best = 0;
    for (int i = 0; i < m; i++) {
        int diag = 0;   // dp value of (i-1, j-1), saved before the row overwrites it
        for (int j = 0; j < n; j++) {
            int up = dp[j + 1];
            if (matrix[i][j] == '1') {
                dp[j + 1] = min(min(up, dp[j]), diag) + 1;
                best = max(best, dp[j + 1]);
            } else {
                dp[j + 1] = 0;
            }
            diag = up;
        }
    }
    return best * best;
}

void test(const string& label, const vector<vector<char>>& matrix, int want) {
    if (maximalSquare(matrix) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("corner-square", {{'1', '1', '0'}, {'1', '1', '1'}, {'1', '1', '1'}}, 4);
    test("diagonal", {{'1', '0'}, {'0', '1'}}, 1);
    test("all-zero", {{'0'}}, 0);
}
