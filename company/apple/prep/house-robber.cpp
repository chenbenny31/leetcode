#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// dp table, T: O(n), S: O(n)
int rob(const vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n); // dp[i] = best loot for 0..i
    dp[0] = nums[0];
    for (int i = 1; i < n; i++) {
        int take = nums[i] + (i >= 2 ? dp[i - 2] : 0);
        dp[i] = max(dp[i - 1], take);
    }
    return dp[n - 1];
}

// keep two state
int robRolling(const vector<int>& nums) {
    int prev = 0; // house i-2
    int cur = 0; // house i-1
    for (int num : nums) {
        int best = max(cur, prev + num);
        prev = cur;
        cur = best;
    }
    return cur;
}

void test(const string& label, const vector<int>& nums, int want) {
    bool ok = rob(nums) == want && robRolling(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("basic", {1, 2, 3}, 4);
    test("skip-two", {2, 1, 1, 2}, 4);
    test("one", {5}, 5);
}
