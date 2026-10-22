// LC 416 Partition Equal Subset Sum: can nums be split into two subsets with equal sums?
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// 0/1 knapsack: does some subset reach half of the total? T: O(n * sum), S: O(sum).
bool canPartition(const vector<int>& nums) {
    int total = 0;
    for (int x : nums) {
        total += x;
    }
    if (total % 2 != 0) { return false; }
    int half = total / 2;
    vector<bool> dp(half + 1, false);   // dp[s] = some subset of items so far sums to s
    dp[0] = true;
    for (int x : nums) {
        // high to low: dp[s - x] is still the row before x, so x is used at most once
        for (int s = half; s >= x; s--) {
            if (dp[s - x]) { dp[s] = true; }
        }
    }
    return dp[half];
}

void test(const string& label, const vector<int>& nums, bool want) {
    if (canPartition(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("split", {3, 1, 1, 3}, true);
    test("odd-total", {1, 2, 4}, false);
    test("unreachable", {1, 1, 6}, false);
}
