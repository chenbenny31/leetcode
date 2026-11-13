// 17 · Subarray Sum Equals K (Prefix Sum + Hash) — LC 560, 523, 974, 1590, 325, 525, 1109
// Mother: select/01-arrays-hashing/02-subarray-sum-k.cpp
// (1) LC 560: count non-empty subarrays w/ sum k (negatives, zeros allowed)
// (2) remove exactly 1 element; can the rest split into prefix / suffix w/ equal sums?
// (3) rects [r1, c1, r2, c2] (inclusive) on m x n grid; max num of rects covering a cell
//
// clarify:
// negative values, k <= 0 -> no sliding window; prefix + hash
// overlapping subarrays -> each counts
// sums -> fit in int
// (2) prefix / suffix -> both non-empty
// (3) corners -> inclusive, inside the grid

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

// (1) count subarrays w/ sum k (LC 560)
// core idea: sum(i..j) = prefix(j + 1) - prefix(i); count earlier prefixes = prefix - k
// T: O(n), S: O(n)
int subarraySum(const vector<int>& nums, int k) {
    // prefixCount[p] = earlier prefixes w/ sum p; seed {0: 1} = empty prefix
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

// (2) remove one element, then split into equal prefix / suffix
// core idea: removed x sits in the bigger part: right x = total - 2L, left x = total - 2R
// one pass per side w/ a set of that side's values; the part losing x needs 2+ elements
// T: O(n), S: O(n)
bool canSplitAfterRemoval(const vector<int>& nums) {
    int n = nums.size();
    if (n < 3) { return false; }
    // prefixSum[c] = sum of the first c elements
    vector<int> prefixSum(n + 1, 0);
    for (int i = 0; i < n; i++) { prefixSum[i + 1] = prefixSum[i] + nums[i]; }
    int total = prefixSum[n];
    // values in nums[c..n-1]; suffix needs 2+ elements to survive a removal
    unordered_set<int> rightVals{nums[n - 1]};
    for (int c = n - 2; c >= 1; c--) {
        rightVals.insert(nums[c]);
        if (rightVals.count(total - 2 * prefixSum[c])) { return true; }
    }
    // values in nums[0..c-1]; prefix needs 2+ elements to survive a removal
    unordered_set<int> leftVals{nums[0]};
    for (int c = 2; c <= n - 1; c++) {
        leftVals.insert(nums[c - 1]);
        int right = total - prefixSum[c];
        if (leftVals.count(total - 2 * right)) { return true; }
    }
    return false;
}

// (3) max rectangle overlap
// core idea: 2D difference array (4 corner updates per rect), then 2D prefix sums
// T: O(rects + m * n), S: O(m * n)
int maxRectangleOverlap(const vector<vector<int>>& rects, int m, int n) {
    // 2D difference array; row m, column n only absorb corners past the grid
    vector<vector<int>> diff(m + 1, vector<int>(n + 1, 0));
    for (const auto& rect : rects) {
        int r1 = rect[0];
        int c1 = rect[1];
        int r2 = rect[2];
        int c2 = rect[3];
        // +1 top-left, -1 right of c2 and below r2, +1 back at far corner
        diff[r1][c1]++;
        diff[r1][c2 + 1]--;
        diff[r2 + 1][c1]--;
        diff[r2 + 1][c2 + 1]++;
    }
    int best = 0;
    for (int r = 0; r < m; r++) {
        for (int c = 0; c < n; c++) {
            // 2D prefix sum: above + left - top-left block counted twice
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

// follow-up:
// longest w/ sum k (LC 325) -> map prefix -> FIRST idx, seed {0: -1}
// equal 0s and 1s (LC 525) -> count 0 as -1, longest zero-sum subarray
// multiple of k, len >= 2 (LC 523) -> earliest idx per prefix % k, seed {0: -1}
// count divisible by k (LC 974) -> count equal remainders ((p % k) + k) % k
// shortest w/ sum % mod == k (LC 1590) -> remainder -> LATEST idx of (prefix - k) % mod
// flight bookings (LC 1109) -> 1D difference array: +seats at first, -seats after last
// all values positive -> sliding window, O(1) space
// equal endpoints -> 18; max ends score -> 19; alternating parity queries -> 54
