// LC 198 House Robber: nums[i] is the cash in house i. Adjacent houses cannot both be
// robbed; return the maximum cash.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::max;
using std::string;
using std::vector;

// 1D DP, dp[i] = max(dp[i-1], dp[i-2] + nums[i]), kept in two variables.
// T: O(n), S: O(1).
int rob(const vector<int>& nums) {
    int prev2 = 0;   // best total over houses up to i-2
    int prev1 = 0;   // best total over houses up to i-1
    for (int x : nums) {
        int cur = max(prev1, prev2 + x);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

void test(const string& label, const vector<int>& nums, int want) {
    if (rob(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("skip-two", {4, 1, 1, 4}, 8);
    test("alternate", {2, 9, 3, 8}, 17);
    test("single", {5}, 5);
    test("empty", {}, 0);
}
