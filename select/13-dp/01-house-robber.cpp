// LC 198 House Robber (Medium). Pattern: 1D DP, take or skip.
// Houses in a row hold nums[i] >= 0; return the max total you can rob without robbing
// two adjacent houses.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// dp[i] = max(dp[i-1], dp[i-2] + nums[i]) in two rolling vars. T: O(n), S: O(1).
int rob(const vector<int>& nums) {
    // best loot up to the house before the previous one
    int prev = 0;
    // best loot up to the previous house
    int cur = 0;
    for (int money : nums) {
        // skip this house, or rob it on top of prev
        int best = max(cur, prev + money);
        prev = cur;
        cur = best;
    }
    return cur;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (rob(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("skip-two", {2, 1, 1, 2}, 4);
    test("middle-big", {1, 3, 1}, 3);
    test("mixed", {3, 10, 3, 1, 2}, 12);
    test("single", {4}, 4);
}

// follow-up: houses in a circle (LC 213) -> first and last can't both be robbed:
//   max(rob(nums[0..n-2]), rob(nums[1..n-1])); n == 1 -> nums[0]
// follow-up: binary tree (LC 337) -> dfs returns {take, skip}: take = val + skip(L) +
//   skip(R), skip = max(L) + max(R); O(n)
