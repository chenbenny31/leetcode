// LC 213 House Robber II: houses stand in a circle, so the first and last are adjacent.
// Adjacent houses cannot both be robbed; return the maximum cash.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::max;
using std::string;
using std::vector;

// House Robber on the line nums[lo, hi). T: O(hi - lo), S: O(1).
int robLine(const vector<int>& nums, int lo, int hi) {
    int prev2 = 0;   // best total over houses up to i-2
    int prev1 = 0;   // best total over houses up to i-1
    for (int i = lo; i < hi; i++) {
        int cur = max(prev1, prev2 + nums[i]);
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

// A circle breaks into two lines: either skip the first house or skip the last one.
// T: O(n), S: O(1).
int rob(const vector<int>& nums) {
    int n = nums.size();
    if (n == 1) { return nums[0]; }
    return max(robLine(nums, 0, n - 1), robLine(nums, 1, n));
}

void test(const string& label, const vector<int>& nums, int want) {
    if (rob(nums) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    test("wrap-adjacent", {4, 1, 1, 4}, 5);
    test("three", {2, 3, 2}, 3);
    test("single", {5}, 5);
}
