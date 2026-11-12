// LC 198 House Robber (Medium). Pattern: 1D DP, take or skip.
// Houses in a row hold nums[i] >= 0; return the max total you can rob without robbing
// two adjacent houses.

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::max;
using std::reverse;

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

// LC 213: first and last are neighbors, so rob the row without the last or without
// the first. T: O(n), S: O(n) for the two copies.
int robCircle(const vector<int>& nums) {
    if (nums.size() == 1) { return nums[0]; }
    vector<int> withoutLast(nums.begin(), nums.end() - 1);
    vector<int> withoutFirst(nums.begin() + 1, nums.end());
    return max(rob(withoutLast), rob(withoutFirst));
}

// Max loot plus the robbed houses ascending: keep the full dp array, then walk back from
// the end, taking house i when dp[i] comes from taking it. T: O(n), S: O(n).
pair<int, vector<int>> robWithHouses(const vector<int>& nums) {
    int n = nums.size();
    if (n == 0) { return {0, {}}; }
    // dp[i] = best loot from houses 0..i
    vector<int> dp(n, 0);
    for (int i = 0; i < n; i++) {
        int skip = i >= 1 ? dp[i - 1] : 0;
        int take = nums[i] + (i >= 2 ? dp[i - 2] : 0);
        dp[i] = max(skip, take);
    }
    vector<int> houses;
    int house = n - 1;
    while (house >= 0) {
        int take = nums[house] + (house >= 2 ? dp[house - 2] : 0);
        if (dp[house] == take) {
            // robbed, so its left neighbor was not
            houses.push_back(house);
            house -= 2;
        } else {
            house--;
        }
    }
    reverse(houses.begin(), houses.end());
    return {dp[n - 1], houses};
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (rob(nums) == want ? " PASS\n" : " FAIL\n");
}

void testCircle(const string& label, const vector<int>& nums, int want) {
    cout << label << (robCircle(nums) == want ? " PASS\n" : " FAIL\n");
}

void testHouses(const string& label, const vector<int>& nums, int want,
                const vector<int>& wantHouses) {
    auto [amount, houses] = robWithHouses(nums);
    bool ok = amount == want && houses == wantHouses;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("skip-two", {2, 1, 1, 2}, 4);
    test("middle-big", {1, 3, 1}, 3);
    test("mixed", {3, 10, 3, 1, 2}, 12);
    test("single", {4}, 4);
    testCircle("ends-touch", {3, 1, 3}, 3);
    testCircle("drop-first", {1, 4, 2, 3}, 7);
    testCircle("circle-single", {5}, 5);
    testHouses("houses-skip-two", {2, 1, 1, 2}, 4, {0, 3});
    testHouses("houses-mixed", {3, 10, 3, 1, 2}, 12, {1, 4});
    testHouses("houses-single", {4}, 4, {0});
    testHouses("houses-empty", {}, 0, {});
}

// follow-up: binary tree (LC 337) -> dfs returns {take, skip}: take = val + skip(L) +
//   skip(R), skip = max(L) + max(R); O(n)
