// 68 · House Robber — LC 198, 213, 70
// Mother: select/13-dp/01-house-robber.cpp
// houses in a row hold nums[i] >= 0; max total w/o robbing two adjacent houses
// reported: return the robbed houses
//
// clarify:
// empty row -> 0
// single house -> take it
// total or the houses -> total; houses as follow-up

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// core idea: dp[i] = max(dp[i-1] skip, dp[i-2] + nums[i] take), kept in two rolling vars
// T: O(n), S: O(1)
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

// follow-up:
// return the robbed houses (Google) -> full dp, walk back: took i if dp[i] != dp[i-1]
// houses in a circle (LC 213) -> max of rob(row w/o last), rob(row w/o first)
// binary tree (LC 337) -> dfs returns {take, skip}: take = val + skip(L) + skip(R)
// climbing stairs (LC 70) -> ways[i] = ways[i-1] + ways[i-2], two rolling vars
