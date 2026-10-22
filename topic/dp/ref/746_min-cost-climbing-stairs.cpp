// LC 746 Min Cost Climbing Stairs: paying cost[i] lets you climb one or two steps from
// step i. Start on step 0 or 1; return the min cost to reach the top (index n).
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::min;
using std::string;
using std::vector;

// 1D DP over steps, dp[i] = min(dp[i-1] + cost[i-1], dp[i-2] + cost[i-2]), two variables.
// T: O(n), S: O(1).
int minCostClimbingStairs(const vector<int>& cost) {
    int n = cost.size();
    int two = 0;   // min cost to stand on step i-2 (steps 0 and 1 are free to start on)
    int one = 0;   // min cost to stand on step i-1
    for (int i = 2; i <= n; i++) {
        int cur = min(one + cost[i - 1], two + cost[i - 2]);
        two = one;
        one = cur;
    }
    return one;
}

void test(const string& label, const vector<int>& cost, int want) {
    if (minCostClimbingStairs(cost) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("skip-expensive", {1, 100, 1, 1, 1, 100}, 3);
    test("alternate", {10, 1, 10, 1}, 2);
    test("two-steps", {5, 2}, 2);
}
