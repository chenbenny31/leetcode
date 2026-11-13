// 16 · Subarray Sum Equals K (Prefix Sum + Hash Map) — LC 560, 523, 974, 1590, 325, 525,
//   1109
// Mother problem: select/01-arrays-hashing/02-subarray-sum-k.cpp
//
// Google reports: max subarray with equal endpoints (17) ×6, split after one removal ×3,
// target remainder ×3, max ends score (18) ×3, alternating parity queries (50) ×3, flight
// bookings ×2, rectangle coverage ×2, subarray with a given mean
//
// LC 560: count the non-empty subarrays whose sum is k; negatives and zeros allowed (so a
// sliding window fails), k may be 0 or negative, overlapping subarrays each count.
//
// Core idea: sum(i..j) = prefix(j + 1) - prefix(i), so count earlier prefixes equal to
// prefix - k. Seed {0: 1}; look up before inserting so k == 0 never counts the empty one.
//
// Follow-ups
// - split after one removal (Google) → `canSplitAfterRemoval` below
// - rectangle coverage (Google) → `maxRectangleOverlap` below (2D difference array)
// - longest with sum k (LC 325) → map prefix → FIRST index, seeded {0: -1}
// - equal 0s and 1s (LC 525) → count 0 as -1, longest zero-sum subarray
// - subarray with a given mean (Google) → subtract mean from each value, longest zero-sum
// - multiple of k, length >= 2 (LC 523) → earliest index per prefix % k, seeded {0: -1}
// - count divisible by k (LC 974) → count equal remainders ((p % k) + k) % k
// - shortest with sum % mod == k (Google, LC 1590) → map remainder → LATEST index, look
//   up (prefix - k + mod) % mod
// - flight bookings (LC 1109) → 1D difference array: +seats at first, -seats after last
// - submatrices with sum k (LC 1074) → fix top/bottom rows, subarraySum on column sums
// - all values positive → sliding window, O(1) space

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::max;

// A subarray ending here sums to k iff an earlier prefix equals prefix - k.
// T: O(n), S: O(n).
int subarraySum(const vector<int>& nums, int k) {
    // prefixCount[p] = earlier prefixes with sum p; the empty prefix counts once
    unordered_map<int, int> prefixCount{{0, 1}};
    int prefix = 0;
    int count = 0;
    for (int num : nums) {
        prefix += num;
        auto it = prefixCount.find(prefix - k);
        if (it != prefixCount.end()) {
            count += it->second;
        }
        // after the lookup, so k == 0 never counts an empty subarray
        prefixCount[prefix]++;
    }
    return count;
}

// Google: remove exactly one element, then cut the rest into a non-empty prefix and a
// non-empty suffix with equal sums; possible? The removed x sits in the bigger part:
// removed on the right, x = total - 2L must be in the suffix; on the left, x = total - 2R
// must be in the prefix. One pass per side with a set of that side's values.
// Pitfall: the part that loses x needs 2+ elements, so each set starts with the outermost
// value and the cut stops one short of that end. T: O(n), S: O(n).
bool canSplitAfterRemoval(const vector<int>& nums) {
    int n = nums.size();
    if (n < 3) { return false; }
    // prefixSum[c] = sum of the first c elements
    vector<int> prefixSum(n + 1, 0);
    for (int i = 0; i < n; i++) { prefixSum[i + 1] = prefixSum[i] + nums[i]; }
    int total = prefixSum[n];
    // values in nums[c..n-1]; the suffix needs 2+ elements to survive a removal
    unordered_set<int> rightVals{nums[n - 1]};
    for (int c = n - 2; c >= 1; c--) {
        rightVals.insert(nums[c]);
        if (rightVals.count(total - 2 * prefixSum[c])) { return true; }
    }
    // values in nums[0..c-1]; the prefix needs 2+ elements to survive a removal
    unordered_set<int> leftVals{nums[0]};
    for (int c = 2; c <= n - 1; c++) {
        leftVals.insert(nums[c - 1]);
        int right = total - prefixSum[c];
        if (leftVals.count(total - 2 * right)) { return true; }
    }
    return false;
}

// Google: rectangles [r1, c1, r2, c2] (0-indexed, inclusive) on an m x n grid; return the
// max number of rectangles covering one cell (0 if none). 2D difference array, then 2D
// prefix sums give each cell's cover count. T: O(rects + m * n), S: O(m * n).
int maxRectangleOverlap(const vector<vector<int>>& rects, int m, int n) {
    // diff = 2D difference array; after the prefix pass diff[r][c] = rectangles covering
    // cell (r, c). Row m and column n only absorb the corners past the grid.
    vector<vector<int>> diff(m + 1, vector<int>(n + 1, 0));
    for (const auto& rect : rects) {
        int r1 = rect[0];
        int c1 = rect[1];
        int r2 = rect[2];
        int c2 = rect[3];
        // +1 turns on everything below-right of (r1, c1); the two -1s cut off below r2
        // and right of c2; the last +1 restores the corner block cut off twice
        diff[r1][c1]++;
        diff[r1][c2 + 1]--;
        diff[r2 + 1][c1]--;
        diff[r2 + 1][c2 + 1]++;
    }
    int best = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            // inclusion-exclusion: add the sums above and to the left, subtract the
            // top-left block that both of them include
            if (r > 0) { diff[r][c] += diff[r - 1][c]; }
            if (c > 0) { diff[r][c] += diff[r][c - 1]; }
            if (r > 0 && c > 0) { diff[r][c] -= diff[r - 1][c - 1]; }
            best = max(best, diff[r][c]);
        }
    }
    return best;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (subarraySum(nums, k) == want ? " PASS\n" : " FAIL\n");
}

void testSplit(const string& label, const vector<int>& nums, bool want) {
    cout << label << (canSplitAfterRemoval(nums) == want ? " PASS\n" : " FAIL\n");
}

void testOverlap(const string& label, const vector<vector<int>>& rects, int m, int n,
                 int want) {
    cout << label << (maxRectangleOverlap(rects, m, n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlap", {1, 2, 1}, 3, 2);
    test("negative-zero-k", {1, -1, 0}, 0, 3);
    test("negative-k", {-1, 1, -1, -1}, -1, 5);
    test("none", {1, 2}, 5, 0);
    testSplit("split-remove-first", {7, 1, 1, 2}, true);
    testSplit("split-remove-last", {2, 1, 1, 7}, true);
    testSplit("split-none", {1, 2, 4}, false);
    testSplit("split-too-short", {1, 1}, false);
    testOverlap("overlap-corner", {{0, 0, 1, 1}, {1, 1, 2, 2}}, 3, 3, 2);
    testOverlap("overlap-nested", {{0, 0, 2, 2}, {0, 0, 1, 1}, {1, 1, 1, 1}}, 3, 3, 3);
    testOverlap("overlap-none", {}, 2, 2, 0);
}
