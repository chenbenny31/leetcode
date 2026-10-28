// LC 3644: nums is a permutation of 0..n-1. You may swap nums[i] and nums[j] only when
// nums[i] & nums[j] == k. Return the max k that lets you sort nums (0 if it is already
// sorted).

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// Every misplaced value takes part in a swap, so k is a bit-subset of each of them:
// k <= AND of misplaced values. That AND works: it is itself a value v <= n - 1 in the
// array, and v & x == v for every misplaced x, so v acts as the swap buffer.
// T: O(n), S: O(1).
int sortPermutation(const vector<int>& nums) {
    int n = nums.size();
    int k = -1;
    for (int i = 0; i < n; i++) {
        if (nums[i] != i) { k &= nums[i]; }
    }
    if (k == -1) { return 0; }
    return k;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (sortPermutation(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("sorted", {0, 1, 2}, 0);
    test("pair-low", {0, 3, 2, 1}, 1);
    test("pair-high", {0, 1, 3, 2}, 2);
    test("cycle", {1, 2, 0}, 0);
}
