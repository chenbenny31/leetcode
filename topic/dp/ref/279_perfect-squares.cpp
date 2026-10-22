// LC 279 Perfect Squares: return the fewest perfect squares (1, 4, 9, ...) that sum to n.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::min;
using std::string;
using std::vector;

// Unbounded-knapsack DP over square numbers. T: O(n * sqrt(n)), S: O(n).
int numSquares(int n) {
    // dp[i] = fewest squares summing to i; n is a safe start since i ones always work
    vector<int> dp(n + 1, n);
    dp[0] = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j * j <= i; j++) {
            dp[i] = min(dp[i], dp[i - j * j] + 1);
        }
    }
    return dp[n];
}

void test(const string& label, int n, int want) {
    if (numSquares(n) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("sum-of-three", 12, 3);
    test("is-square", 16, 1);
    test("needs-four", 7, 4);
    test("one", 1, 1);
}
