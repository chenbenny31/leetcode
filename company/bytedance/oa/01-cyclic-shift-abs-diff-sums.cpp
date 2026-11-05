// ByteDance OA: nums1, nums2 of length n (n <= 200, |values| <= 1e9). For each right
// cyclic shift s of nums1, sum |nums1[(i - s + n) % n] - nums2[i]| over all i.
// Return the n sums sorted ascending.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::abs;

// Brute force every shift; n <= 200 keeps n^2 small. T: O(n^2), S: O(n).
vector<long long> sortedCyclicShiftDifferences(const vector<int>& nums1,
                                               const vector<int>& nums2) {
    int n = nums1.size();
    // sums[s] = total |difference| when nums1 is shifted right by s
    vector<long long> sums(n, 0);
    for (int s = 0; s < n; s++) {
        for (int i = 0; i < n; i++) {
            // the gap can reach 2e9, so subtract in 64 bits
            long long diff = (long long)nums1[(i - s + n) % n] - nums2[i];
            sums[s] += abs(diff);
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

// follow-up: only the best shift -> track the min while looping, skip the sort
// follow-up: also return the shift -> sort vector<pair<long long, int>> of (sum, s)
// follow-up: squared diff, large n -> sum a^2 + sum b^2 - 2 * cross-correlation of a
//   and b, all shifts at once via FFT, O(n log n)
