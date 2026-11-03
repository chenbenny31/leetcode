// ByteDance (1p3a infra onsite, "伞药耳") / LC 312: balloons in a row carry numbers
// nums[i]. Bursting balloon i earns nums[left] * nums[i] * nums[right], where left and
// right are its current neighbors (a missing neighbor counts as 1); then the two
// neighbors become adjacent. Return the maximum total by bursting all balloons.
// 1 <= n <= 300, 0 <= nums[i] <= 100.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Pad with a 1 on both ends. dp[l][r] = best total for bursting every balloon strictly
// between l and r while l and r stay. Think of the LAST balloon k burst in (l, r): at
// that moment its neighbors are exactly l and r, and the two sides were solved
// independently. Fill by increasing gap r - l. T: O(n^3), S: O(n^2).
int maxCoins(const vector<int>& nums) {
    vector<int> vals;
    vals.push_back(1);
    for (int x : nums) {
        vals.push_back(x);
    }
    vals.push_back(1);
    int m = vals.size();

    // dp[l][r] = max coins from bursting all balloons strictly between l and r
    vector<vector<int>> dp(m, vector<int>(m, 0));
    for (int gap = 2; gap < m; gap++) {
        for (int l = 0; l + gap < m; l++) {
            int r = l + gap;
            for (int k = l + 1; k < r; k++) {
                int coins = dp[l][k] + vals[l] * vals[k] * vals[r] + dp[k][r];
                dp[l][r] = max(dp[l][r], coins);
            }
        }
    }
    return dp[0][m - 1];
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (maxCoins(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one", {5}, 5);
    test("two", {1, 5}, 10);
    test("three", {2, 3, 4}, 36);
    test("zero-inside", {4, 0, 2}, 12);
}
