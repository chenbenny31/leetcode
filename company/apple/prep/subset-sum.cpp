// judge if any subset's sum == target in a non-negative array

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// 0/1 knapsack, pick or not each element to makeup target-sum (dp-range)
// T: O(n * target), S: O(target)
bool canSum(const vector<int>& arr, int target) {
    vector<bool> dp(target + 1, false); // dp[i] = exist subset make up sum==i
    dp[0] = true;
    for (int val : arr) {
        // from high to low: val is used at most once for makeup cur sum
        for (int sum = target; sum >= val; sum--) {
            if (dp[sum - val]) {
                dp[sum] = true;
            }
        }
    }
    return dp[target];
}

void test(const string& label, const vector<int>& arr, int target, bool want) {
    cout << label << (canSum(arr, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("example", {3, 34, 4, 12, 5, 2}, 9, true);
    test("too big", {3, 34, 4, 12, 5, 2}, 30, false);
    test("no reuse", {5}, 10, false);
    test("target 0", {}, 0, true);
}
