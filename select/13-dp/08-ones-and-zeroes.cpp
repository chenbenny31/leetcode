// LC 474 Ones and Zeroes (Medium). Pattern: 0/1 knapsack with two capacities.
// strs are binary strings; pick the largest subset with at most m '0's and n '1's in
// total. Return the subset size.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// dp[i][j] = largest subset using at most i zeros and j ones. Each string is an item
// with weight (zeros, ones) and value 1; loop capacities downward so it is used once.
// T: O(L * m * n) for L strings, S: O(m * n).
int findMaxForm(const vector<string>& strs, int m, int n) {
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (const string& str : strs) {
        int zeros = 0;
        int ones = 0;
        for (char ch : str) {
            if (ch == '0') {
                zeros++;
            } else {
                ones++;
            }
        }
        // downward, like 1D 0/1 knapsack: dp[i - zeros][j - ones] is still "without str"
        for (int i = m; i >= zeros; i--) {
            for (int j = n; j >= ones; j--) {
                dp[i][j] = max(dp[i][j], dp[i - zeros][j - ones] + 1);
            }
        }
    }
    return dp[m][n];
}

void test(const string& label, const vector<string>& strs, int m, int n, int want) {
    cout << label << (findMaxForm(strs, m, n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("pick-small-ones", {"0", "1", "01", "0011"}, 2, 2, 3);
    test("zero-budget", {"0", "1"}, 0, 1, 1);
    test("nothing-fits", {"00", "11"}, 1, 1, 0);
}

// follow-up: why loop downward -> upward would let dp[i - zeros][j - ones] already
//   include str, i.e. use the same string twice (unbounded knapsack)
// follow-up: 1D version of the same idea -> LC 416 partition equal subset sum
