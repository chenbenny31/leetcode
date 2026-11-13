// LC 198 House Robber (Medium) — 1D DP, take or skip
// houses in a row hold nums[i] >= 0; return max total w/o robbing two adjacent houses
//
// clarify:
// values >= 0 -> dp never decreases, no negative houses to avoid
// empty input -> 0
// total fits in int? -> assume yes, else long long
// amount only, not the houses -> rolling vars; houses -> follow-up

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// core idea: dp[i] = max(dp[i - 1], dp[i - 2] + nums[i]); keep two rolling vars
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
// houses in a circle (LC 213) -> max(rob w/o last house, rob w/o first); n = 1 own case
// return the robbed houses -> full dp; walk back: robbed if dp[i] == nums[i] + dp[i - 2]
// houses on a binary tree (LC 337) -> dfs returns {take, skip}, O(n)
// climbing stairs (LC 70) / min cost (LC 746) -> same two-var recurrence
