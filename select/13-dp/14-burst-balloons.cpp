// LC 312 Burst Balloons (Hard). Pattern: interval DP on the last element.
// Bursting balloon i earns left * nums[i] * right with its current neighbors (missing
// neighbor = 1); then those two become adjacent. Return the max total for bursting all.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Pick the LAST balloon burst in (left, right): its neighbors are then exactly left and
// right. T: O(n^3), S: O(n^2).
int maxCoins(const vector<int>& nums) {
    // nums padded with a 1 on both ends
    vector<int> vals;
    vals.push_back(1);
    for (int num : nums) {
        vals.push_back(num);
    }
    vals.push_back(1);
    int n = vals.size();

    // dp[left][right] = max coins from bursting every balloon strictly between them
    vector<vector<int>> dp(n, vector<int>(n, 0));
    // shorter intervals first, so dp[left][last] and dp[last][right] are ready
    for (int gap = 2; gap < n; gap++) {
        for (int left = 0; left + gap < n; left++) {
            int right = left + gap;
            for (int last = left + 1; last < right; last++) {
                int burstLast = vals[left] * vals[last] * vals[right];
                int coins = dp[left][last] + burstLast + dp[last][right];
                dp[left][right] = max(dp[left][right], coins);
            }
        }
    }
    return dp[0][n - 1];
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (maxCoins(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one", {5}, 5);
    test("two", {2, 3}, 9);
    test("three", {2, 3, 4}, 36);
    test("zero-inside", {4, 0, 2}, 12);
}

// follow-up: why "last" not "first" -> if it bursts first its neighbors become adjacent
//   and the two halves interact; bursting it last keeps left and right as its neighbors
// follow-up: same shape -> min cost to cut a stick (LC 1547), matrix chain multiplication
