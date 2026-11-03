// ByteDance OA: nums1 and nums2 have the same length n. For every right cyclic shift s
// in [0, n-1] of nums1, nums2[i] is compared with nums1[(i - s + n) % n]; the value of
// the shift is the sum of |difference| over all i. Return the n sums sorted in
// non-decreasing order. Use 64-bit arithmetic (|values| <= 1e9, n <= 200).

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Try every shift and sum the differences directly; n <= 200 means only n^2 = 40000
// steps. T: O(n^2), S: O(n).
vector<long long> sortedCyclicShiftDifferences(const vector<int>& nums1,
                                               const vector<int>& nums2) {
    int n = nums1.size();
    // sums[s] = total |difference| when nums1 is shifted right by s
    vector<long long> sums(n, 0);
    for (int s = 0; s < n; s++) {
        for (int i = 0; i < n; i++) {
            // the gap can reach 2e9, so subtract in 64 bits
            long long diff = (long long)nums1[(i - s + n) % n] - nums2[i];
            if (diff < 0) {
                diff = -diff;
            }
            sums[s] += diff;
        }
    }
    sort(sums.begin(), sums.end());
    return sums;
}

void test(const string& label, const vector<int>& nums1, const vector<int>& nums2,
          const vector<long long>& want) {
    bool ok = sortedCyclicShiftDifferences(nums1, nums2) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("same-array", {1, 2, 3}, {1, 2, 3}, {0, 4, 4});
    test("single", {5}, {2}, {3});
    test("overflow-32", {-1000000000, 1000000000}, {1000000000, -1000000000},
         {0, 4000000000LL});
}
