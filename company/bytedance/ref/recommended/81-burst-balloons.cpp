// ByteDance (1p3a infra onsite) / LC 312: bursting balloon i earns left * nums[i] *
// right, where left / right are its current neighbors (missing = 1); then those two
// become adjacent. Return the max total from bursting all balloons.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Interval DP on the LAST balloon k burst in (l, r): its neighbors are then l and r.
// T: O(n^3), S: O(n^2).
int maxCoins(const vector<int>& nums) {
    // nums padded with a 1 on both ends
    vector<int> vals;
    vals.push_back(1);
    for (int x : nums) {
        vals.push_back(x);
    }
    vals.push_back(1);
    int m = vals.size();

    // dp[l][r] = max coins from bursting all balloons strictly between l and r
    vector<vector<int>> dp(m, vector<int>(m, 0));
    // shorter intervals first, so dp[l][k] and dp[k][r] are ready
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

// follow-up: burst order -> store split[l][r] = best k; emit order(l, k), order(k, r),
//   then k (k is burst last in (l, r))
// follow-up: why "last" not "first" -> if k bursts first its two neighbors become
//   adjacent and the halves interact; last keeps l, r as fixed neighbors
// follow-up: same interval DP -> min cost to cut a stick (LC 1547), matrix chain;
//   dp[i][j] = best over k of dp[i][k] + dp[k][j] + cost(i, k, j)
