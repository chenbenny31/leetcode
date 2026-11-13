// 68 · House Robber — LC 198, 213, 70
// Mother problem: select/13-dp/01-house-robber.cpp
//
// LC 198 (Medium), 1D DP take or skip: houses in a row hold nums[i] >= 0; return the max
// total you can rob without robbing two adjacent houses.
// Google reports: return the robbed houses
//
// Clarify
// - empty row → 0; a single house → its value; return the total only
//
// Core idea
// - dp[i] = max(dp[i-1] skip, dp[i-2] + nums[i] take), in two rolling vars.
// - T O(n), S O(1).
//
// Tests: two skipped in a row, middle big, mixed, single house
//
// Follow-ups
// - houses in a circle (LC 213) → max of rob(row without last), rob(row without first)
// - return the robbed houses → full dp array, walk back from the end: take i and jump
//   to i - 2 when dp[i] == nums[i] + dp[i-2], else step to i - 1
// - binary tree (LC 337) → dfs returns {take, skip}: take = val + skip(L) + skip(R),
//   skip = max(L) + max(R)
// - climbing stairs (LC 70) → ways[i] = ways[i-1] + ways[i-2], two rolling vars
//
//////////////////////////////////////////////////////////////////////////////////////////

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
