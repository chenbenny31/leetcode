// LC 312 Burst Balloons: bursting balloon i earns nums[i-1] * nums[i] * nums[i+1], with
// 1 beyond the ends. Burst all balloons in the best order; return the max coins.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::max;
using std::string;
using std::vector;

// Interval DP choosing the LAST balloon k burst in (l, r), so its neighbors are l and r.
// T: O(n^3), S: O(n^2).
int maxCoins(const vector<int>& nums) {
    int n = nums.size();
    vector<int> val(n + 2, 1);   // nums padded with a 1 on each side
    for (int i = 0; i < n; i++) {
        val[i + 1] = nums[i];
    }
    // dp[l][r] = max coins from bursting every balloon strictly between l and r
    vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));
    for (int len = 2; len <= n + 1; len++) {
        for (int l = 0; l + len <= n + 1; l++) {
            int r = l + len;
            for (int k = l + 1; k < r; k++) {
                int coins = dp[l][k] + val[l] * val[k] * val[r] + dp[k][r];
                dp[l][r] = max(dp[l][r], coins);
            }
        }
    }
    return dp[0][n + 1];
}

void test(const string& label, const vector<int>& nums, int want) {
    if (maxCoins(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("three", {3, 1, 5}, 35);
    test("pair", {2, 3}, 9);
    test("single", {5}, 5);
    test("empty", {}, 0);
}
