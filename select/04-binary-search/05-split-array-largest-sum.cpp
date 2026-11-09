// LC 410 Split Array Largest Sum (Hard). Pattern: binary search on the answer + greedy.
// Split nums (non-negative) into k non-empty contiguous subarrays so that the largest
// subarray sum is minimized; return that sum.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// Greedy: fewest subarrays so that no subarray sum exceeds cap (cap >= max element).
// T: O(n), S: O(1).
int piecesNeeded(const vector<int>& nums, int cap) {
    int pieces = 1;
    // sum of the current piece
    int sum = 0;
    for (int num : nums) {
        if (sum + num > cap) {
            pieces++;
            sum = 0;
        }
        sum += num;
    }
    return pieces;
}

// Lower-bound template on cap in [max element, total]: pieces <= k is monotonic in cap,
// and fewer than k pieces can always be split further. T: O(n log(total)), S: O(1).
int splitArray(const vector<int>& nums, int k) {
    int lo = 0;
    int hi = 0;
    for (int num : nums) {
        lo = max(lo, num);
        hi += num;
    }
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (piecesNeeded(nums, mid) <= k) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (splitArray(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-parts", {1, 2, 3, 4}, 2, 6);
    test("one-part", {1, 2, 3, 4}, 1, 10);
    test("each-alone", {1, 2, 3, 4}, 4, 4);
    test("big-first", {9, 1, 1, 1}, 2, 9);
}

// follow-up: DP alternative -> dp[i][j] = best for first i values in j parts,
//   min over split p of max(dp[p][j-1], sum(p..i-1)), O(k n^2)
// follow-up: same shape -> ship packages within D days (LC 1011), Koko (LC 875)
