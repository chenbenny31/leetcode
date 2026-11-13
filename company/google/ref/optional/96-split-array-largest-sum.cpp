// 96 · Split Array Largest Sum — LC 410
// Mother: select/optional/04-binary-search/05-split-array-largest-sum.cpp
// split non-negative nums into k non-empty contiguous subarrays; min the largest sum
// reported: LC tag #19
//
// clarify:
// 1 <= k <= n -> always feasible
// values non-negative -> greedy check is monotonic
// total overflow int? -> assume fits; else long long

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::max;

// greedy: fewest pieces w/ no piece sum > cap (cap >= max element)
int piecesNeeded(const vector<int>& nums, int cap) {
    int pieces = 1;
    // sum of current piece
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

// core idea: binary search cap in [max element, total]; pieces needed <= k is monotonic
// fewer than k pieces can always be split further
// T: O(n log(total)), S: O(1)
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

// follow-up:
// DP alternative -> dp[i][j] = min over cut p of max(dp[p][j-1], sum[p..i)); O(k n^2)
// same shape (LC 1011 ship packages, LC 875 Koko) -> binary search answer + greedy check
