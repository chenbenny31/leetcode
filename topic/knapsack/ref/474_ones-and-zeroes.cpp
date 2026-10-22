// LC 474 Ones and Zeroes: given binary strings, return the size of the largest subset
// with at most m zeros and at most n ones in total.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// 0/1 knapsack with two capacities; iterate both capacities downward so each string
// is used once. T: O(len * m * n), S: O(m * n).
int findMaxForm(const vector<string>& strs, int m, int n) {
    // dp[i][j] = most strings using at most i zeros and j ones
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (const auto& s : strs) {
        int zeros = 0;
        int ones = 0;
        for (char c : s) {
            if (c == '0') {
                zeros++;
            } else {
                ones++;
            }
        }
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
    test("pick-two", {"00", "11", "01"}, 2, 2, 2);
    test("too-big", {"11", "00"}, 1, 1, 0);
    test("use-once", {"0"}, 5, 5, 1);
    test("empty", {}, 3, 3, 0);
}
