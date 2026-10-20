// LC 198: houses in a row hold nums[i]; max total without robbing two adjacent houses.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// straightforward: dp table; house i is either skipped or taken on top of i-2.
// T: O(n), S: O(n).
int rob(const vector<int>& nums) {
    int n = nums.size();
    vector<int> dp(n);   // dp[i] = best loot from houses 0..i
    dp[0] = nums[0];
    for (int i = 1; i < n; i++) {
        int take = nums[i] + (i >= 2 ? dp[i - 2] : 0);
        dp[i] = max(dp[i - 1], take);
    }
    return dp[n - 1];
}

// improved: dp[i] only reads dp[i-1] and dp[i-2], so keep two values.
// T: O(n), S: O(1).
int robRolling(const vector<int>& nums) {
    int prev = 0;   // best loot up to house i-2
    int cur = 0;    // best loot up to house i-1
    for (int x : nums) {
        int best = max(cur, prev + x);   // skip x, or take x on top of i-2
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
