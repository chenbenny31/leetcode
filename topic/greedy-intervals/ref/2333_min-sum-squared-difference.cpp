// LC 2333 Minimum Sum of Squared Difference: minimize the sum of (nums1[i] - nums2[i])^2
// with at most k1 +1/-1 changes to nums1 and k2 to nums2.
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::abs;
using std::max;
using std::min;

// Only the gaps |nums1[i] - nums2[i]| matter, and k1 + k2 unit decrements can go to any
// gap. Lowering the largest gap saves the most, so level the top down one value at a
// time with a count per gap. T: O(n + D) for the largest gap D, S: O(D).
long long minSumSquareDiff(const vector<int>& nums1, const vector<int>& nums2, int k1,
                           int k2) {
    int n = nums1.size();
    long long k = 1LL * k1 + k2;
    int maxGap = 0;
    for (int i = 0; i < n; i++) { maxGap = max(maxGap, abs(nums1[i] - nums2[i])); }
    vector<long long> cnt(maxGap + 1, 0);   // cnt[g] = indices whose gap is g
    for (int i = 0; i < n; i++) { cnt[abs(nums1[i] - nums2[i])]++; }

    for (int g = maxGap; g > 0 && k > 0; g--) {
        long long moved = min(cnt[g], k);   // gaps lowered from g to g - 1
        cnt[g] -= moved;
        cnt[g - 1] += moved;
        k -= moved;
    }

    long long total = 0;
    for (int g = 1; g <= maxGap; g++) { total += cnt[g] * g * g; }
    return total;
}

void test(const string& label, const vector<int>& nums1, const vector<int>& nums2,
          int k1, int k2, long long want) {
    cout << label << (minSumSquareDiff(nums1, nums2, k1, k2) == want ? " PASS\n"
                                                                     : " FAIL\n");
}

int main() {
    test("level-top", {1, 4, 10}, {1, 1, 1}, 3, 2, 25);
    test("tie-split", {5, 5}, {0, 0}, 1, 2, 25);
    test("enough-ops", {1, 2}, {2, 4}, 1000000000, 1000000000, 0);
}
