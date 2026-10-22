// LC 53 Maximum Subarray: return the largest sum of a non-empty contiguous subarray.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

// Kadane: drop the running prefix as soon as it is negative. T: O(n), S: O(1).
int maxSubArray(const vector<int>& nums) {
    int best = nums[0];   // best sum seen so far
    int cur = 0;          // best sum of a subarray ending at the current index
    for (int x : nums) {
        cur = max(cur, 0) + x;
        best = max(best, cur);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (maxSubArray(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("restart", {-2, 1, -3, 4}, 4);
    test("all-negative", {-3, -1, -2}, -1);
    test("single", {5}, 5);
}
