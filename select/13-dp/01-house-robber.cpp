// LC 198 House Robber (Medium) — 1D DP, take or skip
// Houses in a row hold nums[i] >= 0; return the max total you can rob without robbing
// two adjacent houses.
// Clarify: values non-negative; empty input returns 0; the total fits in int; only
//   the amount is needed, not the houses.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Core idea: dp[i] = max(dp[i - 1], dp[i - 2] + nums[i]) (skip house i, or rob it on top
// of the best two houses back), kept in two rolling variables. T: O(n), S: O(1).
int rob(const vector<int>& nums) {
    // best loot up to the house before the previous one
    int prev = 0;
    // best loot up to the previous house
    int cur = 0;
    for (int money : nums) {
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

// follow-up (ideas only):
// - houses in a circle (LC 213) -> first and last are neighbors, so answer = max(rob
//   without the last house, rob without the first); one house is its own case
// - also return the robbed houses -> keep the full dp array, walk back from the end:
//   house i was robbed when dp[i] == nums[i] + dp[i - 2], then jump to i - 2, else i - 1
// - houses on a binary tree (LC 337) -> dfs returns {take, skip}: take = val + skip(L)
//   + skip(R), skip = max(L) + max(R); O(n)
// - climbing stairs (LC 70) / min cost climbing stairs (LC 746) -> the same two-variable
//   recurrence: ways[i] = ways[i-1] + ways[i-2] / cost[i] + min of the previous two
