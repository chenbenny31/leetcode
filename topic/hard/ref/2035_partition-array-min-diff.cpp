// LC 2035 Partition Array Into Two Arrays to Minimize Sum Difference: nums has 2n
// integers. Split it into two arrays of n integers each to minimize the absolute
// difference of their sums, and return that minimum.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
#include <cstdlib>
using std::cout;
using std::string;
using std::vector;
using std::lower_bound;
using std::min;
using std::sort;
using std::abs;

// Meet in the middle: enumerate every subset sum of each half, grouped by subset size c.
// Picking c from the left and n-c from the right, binary search the right sums for the
// one that brings the picked total closest to half. T: O(2^n * n), S: O(2^n).
int minimumDifference(vector<int>& nums) {
    int n = nums.size() / 2;
    int total = 0;
    for (int x : nums) { total += x; }
    // left[c] / right[c] = sums of the subsets with c elements of the first / second half
    vector<vector<int>> left(n + 1), right(n + 1);
    for (int mask = 0; mask < (1 << n); mask++) {
        int c = 0, sl = 0, sr = 0;
        for (int i = 0; i < n; i++) {
            if (mask >> i & 1) {
                c++;
                sl += nums[i];
                sr += nums[n + i];
            }
        }
        left[c].push_back(sl);
        right[c].push_back(sr);
    }
    int best = INT_MAX;
    for (int c = 0; c <= n; c++) {
        vector<int>& rs = right[n - c];
        sort(rs.begin(), rs.end());
        for (int l : left[c]) {
            // want l + r close to total / 2, so r close to total / 2 - l
            auto it = lower_bound(rs.begin(), rs.end(), total / 2 - l);
            if (it != rs.end()) { best = min(best, abs(total - 2 * (l + *it))); }
            if (it != rs.begin()) { best = min(best, abs(total - 2 * (l + *(it - 1)))); }
        }
    }
    return best;
}

void test(const string& label, vector<int> nums, int want) {
    cout << label << (minimumDifference(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("negative-mix", {-3, 1, 2, 4}, 2);
    test("two-elements", {5, 8}, 3);
    test("cancel-out", {-4, 4}, 8);
    test("zero-diff", {1, 2, 3, 4, 5, 5}, 0);
}
