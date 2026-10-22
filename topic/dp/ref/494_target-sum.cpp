// LC 494 Target Sum: put + or - before each number of nums. Return how many sign
// assignments make the expression equal target.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// With P = the + group, P - (total - P) = target, so P = (total + target) / 2: count
// subsets with that sum. T: O(n * sum), S: O(sum).
int findTargetSumWays(const vector<int>& nums, int target) {
    int total = 0;
    for (int x : nums) {
        total += x;
    }
    int diff = total + target;
    if (diff < 0 || diff % 2 != 0) { return 0; }
    int want = diff / 2;
    // dp[s] = number of subsets of items so far summing to s
    vector<int> dp(want + 1, 0);
    dp[0] = 1;
    for (int x : nums) {
        // high to low so each item is used at most once; a 0 doubles every count
        for (int s = want; s >= x; s--) {
            dp[s] += dp[s - x];
        }
    }
    return dp[want];
}

void test(const string& label, const vector<int>& nums, int target, int want) {
    if (findTargetSumWays(nums, target) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("zero-sum", {1, 1}, 0, 2);
    test("zeros", {0, 1}, 1, 2);
    test("too-big", {1, 2}, 5, 0);
    test("odd-parity", {1, 2}, 0, 0);
}
