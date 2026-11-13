// 96 · Split Array Largest Sum — LC 410
// Mother problem: select/optional/04-binary-search/05-split-array-largest-sum.cpp
//
// Google reports: LC tag #19
//
// LC 410: split nums (non-negative) into k non-empty contiguous subarrays so that the
// largest subarray sum is minimized; return that sum.
//
// Clarify
// - 1 ≤ k ≤ n, every subarray non-empty and contiguous
// - overflow? → total fits in int for LC limits; use long long if sums can be larger
//
// Core idea
// - Binary search the cap in [max element, total]: "pieces needed ≤ k" is monotonic in
//   cap.
// - `piecesNeeded`: greedy, start a new piece when adding the next value would exceed
//   cap. Fewer than k pieces is fine: any piece can be split further.
// - T O(n log(total)), S O(1).
//
// Tests
// - {1,2,3,4}: k = 2 → 6, k = 1 → 10, k = n → 4; big first {9,1,1,1}, k = 2 → 9
//
// Follow-ups
// - DP alternative → dp[i][j] = best for the first i values in j parts = min over split
//   p of max(dp[p][j−1], sum(p..i−1)), O(k n^2)
// - same shape → ship packages within D days (LC 1011), Koko bananas (LC 875)

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
