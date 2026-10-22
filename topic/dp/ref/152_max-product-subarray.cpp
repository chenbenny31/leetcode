// LC 152 Maximum Product Subarray: given an integer array, find the contiguous
// non-empty subarray with the largest product and return that product.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::max;
using std::min;
using std::string;
using std::swap;
using std::vector;

// Track the max and min product ending here; a negative swaps them. T: O(n), S: O(1).
int maxProduct(const vector<int>& nums) {
    int n = nums.size();
    // mx / mn = largest / smallest product of a subarray ending at the current index
    int mx = nums[0], mn = nums[0], best = nums[0];
    for (int i = 1; i < n; i++) {
        int x = nums[i];
        if (x < 0) {
            swap(mx, mn);
        }
        mx = max(x, mx * x);
        mn = min(x, mn * x);
        best = max(best, mx);
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (maxProduct(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-negatives", {-2, 3, -4}, 24);
    test("single-negative", {-3}, -3);
    test("zero-resets", {-2, 0, -1}, 0);
    test("zero-split", {2, 0, 3, 4}, 12);
}
