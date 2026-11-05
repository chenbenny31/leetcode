// ByteDance (hack2hire) / LC 198: houses in a row hold nums[i]; max total without
// robbing two adjacent houses.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// dp with two rolling values. T: O(n), S: O(1).
int rob(const vector<int>& nums) {
    // best loot up to house i-2
    int prev = 0;
    // best loot up to house i-1
    int cur = 0;
    for (int x : nums) {
        // skip x, or take x on top of i-2
        int best = max(cur, prev + x);
        prev = cur;
        cur = best;
    }
    return cur;
}

// follow-up (LC 213, circle): first and last can't both be robbed, so drop one end.
// T: O(n), S: O(n).
int robCircle(const vector<int>& nums) {
    int n = nums.size();
    if (n == 1) {
        return nums[0];
    }
    vector<int> noLast(nums.begin(), nums.end() - 1);
    vector<int> noFirst(nums.begin() + 1, nums.end());
    return max(rob(noLast), rob(noFirst));
}

void test(const string& label, const vector<int>& nums, int want, int wantCircle) {
    bool ok = rob(nums) == want && robCircle(nums) == wantCircle;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", {1, 2, 3, 1}, 4, 4);
    test("ends-clash", {2, 3, 2}, 4, 3);
    test("skip-two", {2, 1, 1, 2}, 4, 3);
    test("single", {5}, 5, 5);
}

// follow-up: circle (LC 213) -> see robCircle() above; max(rob without last,
//   rob without first)
// follow-up: which houses -> keep dp[], walk back from n - 1: take i iff
//   dp[i] != dp[i - 1], then jump to i - 2
// follow-up: binary tree (LC 337) -> dfs returns {take, skip}: take = val + skip(L) +
//   skip(R), skip = max(L) + max(R); O(n)
