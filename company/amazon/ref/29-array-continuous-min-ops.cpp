// LC 2009: one operation replaces any element with any integer. An array of length n is
// continuous if its values are distinct and max - min == n - 1. Return the min number of
// operations to make nums continuous.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::unique;
using std::min;

// Keep as many distinct values as fit in some range [v, v + n - 1] and replace the rest.
// The best range starts at a kept value, so slide over the sorted distinct values.
// T: O(n log n), S: O(n).
int minOperations(vector<int> nums) {
    int n = nums.size();
    sort(nums.begin(), nums.end());
    nums.erase(unique(nums.begin(), nums.end()), nums.end());
    int u = nums.size();
    int best = n;
    int j = 0;
    for (int i = 0; i < u; i++) {
        // nums[i..j) = distinct values inside [nums[i], nums[i] + n - 1]
        while (j < u && (long long)nums[j] <= (long long)nums[i] + n - 1) { j++; }
        best = min(best, n - (j - i));
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int want) {
    cout << label << (minOperations(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("already", {7, 5, 6}, 0);
    test("one-gap", {1, 3, 5}, 1);
    test("dups", {1, 1, 1}, 2);
    test("far", {1, 10, 100, 1000}, 3);
    test("cluster", {1, 2, 3, 10, 11}, 2);
}
