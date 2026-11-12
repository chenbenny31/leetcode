// LC 560 Subarray Sum Equals K (Medium). Pattern: prefix sum + hash map count.
// Count the contiguous non-empty subarrays of nums whose sum equals k; values may be
// negative, so a sliding window does not work.
//
// Clarify:
// - sizes? -> n up to 2e4, |nums[i]| <= 1000, so every prefix sum fits in int (2e7);
//   trying all (start, end) pairs is O(n^2), the hash map makes it O(n)
// - negatives and zeros -> allowed; the window sum is then not monotonic in its size,
//   which is why two pointers fail
// - k == 0 or k < 0 -> allowed; only non-empty subarrays count
// - count (start, end) pairs: overlapping subarrays each count
// - empty nums -> 0
// Idea: sum(i..j) = prefix(j + 1) - prefix(i). Walking j left to right, the subarrays
//   that end at j and sum to k match the earlier prefixes equal to prefix - k, so a hash
//   map from prefix value to how often it occurred answers each j in O(1).
// Pitfalls:
// - seed the map with {0: 1} (the empty prefix), or subarrays starting at index 0 are
//   lost
// - look up prefix - k before adding prefix, or k == 0 counts the empty subarray
// - look up prefix - k, not k - prefix
// - add it->second (every earlier copy of that prefix), not 1
// Tests to write: overlapping matches; negatives with k == 0; negative k; no match.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

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
// non-empty suffix with equal sums; possible? Try each removal, scan the cuts.
// Clarify: values may be negative; the removed element may sit right next to the cut;
//   n < 3 -> false (each part needs a kept element).
// Pitfalls: both parts stay non-empty after the removal (leftCount < n - 1); compare
//   2 * left == rest instead of dividing rest by 2 (an odd rest never splits).
// Tests to write: removal at the front, at the back, in the middle; no split; n == 2.
// T: O(n^2), S: O(1).
bool canSplitAfterRemoval(const vector<int>& nums) {
    int n = nums.size();
    int total = 0;
    for (int num : nums) { total += num; }
    for (int removed = 0; removed < n; removed++) {
        int rest = total - nums[removed];
        int left = 0;
        // kept elements in the left part so far
        int leftCount = 0;
        for (int i = 0; i < n; i++) {
            if (i == removed) { continue; }
            left += nums[i];
            leftCount++;
            // the right part keeps n - 1 - leftCount >= 1 elements
            if (leftCount < n - 1 && 2 * left == rest) { return true; }
        }
    }
    return false;
}

// The removed x sits in the bigger part: with left sum L and right sum R, x = R - L.
// Removed on the right: x = total - 2L must be in the suffix; on the left: x = total - 2R
// must be in the prefix. One pass per side with a set of that side's values.
// Pitfall: the part that loses x needs 2+ elements, so each set starts with the outermost
// value and the cut stops one short of that end.
// T: O(n), S: O(n).
bool canSplitAfterRemovalLinear(const vector<int>& nums) {
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

// Google: shortest non-empty subarray with sum % mod == k; nums non-negative,
// 0 <= k < mod; -1 if none. Need an earlier prefix with remainder cur - k (mod mod);
// the latest such prefix gives the shortest subarray ending here.
// Clarify: negative values -> normalize each remainder with ((x % mod) + mod) % mod;
//   length = number of elements; -1 when no subarray works.
// Pitfalls: seed {0: 0} (the empty prefix has length 0); overwrite the index so the map
//   keeps the LATEST one (the longest-subarray follow-up keeps the first); + mod before
//   the % keeps need >= 0.
// Tests to write: the latest prefix wins; only the whole array works; none -> -1.
// T: O(n), S: O(min(n, mod)).
int shortestWithRemainder(const vector<int>& nums, int mod, int k) {
    // lastIndex[r] = latest prefix length whose sum % mod == r; the empty prefix is 0
    unordered_map<int, int> lastIndex{{0, 0}};
    int n = nums.size();
    int prefix = 0;
    int best = -1;
    for (int i = 1; i <= n; i++) {
        prefix = (prefix + nums[i - 1]) % mod;
        int need = (prefix - k + mod) % mod;
        auto it = lastIndex.find(need);
        if (it != lastIndex.end() && (best == -1 || i - it->second < best)) {
            best = i - it->second;
        }
        // after the lookup, so the subarray is never empty
        lastIndex[prefix] = i;
    }
    return best;
}

// follow-up (LC 1074): count the submatrices whose sum is k. Fix the top and bottom rows,
// collapse the band into column sums, and count 1D subarrays with sum k in that row.
// T: O(m^2 * n), S: O(n). Put the smaller dimension in the outer pair of loops.
int countSubmatricesSumK(const vector<vector<int>>& matrix, int k) {
    int m = matrix.size();
    int n = matrix[0].size();
    int count = 0;
    for (int top = 0; top < m; top++) {
        // colSum[c] = sum of matrix[top..bottom][c]
        vector<int> colSum(n, 0);
        for (int bottom = top; bottom < m; bottom++) {
            for (int c = 0; c < n; c++) { colSum[c] += matrix[bottom][c]; }
            count += subarraySum(colSum, k);
        }
    }
    return count;
}

void test(const string& label, const vector<int>& nums, int k, int want) {
    cout << label << (subarraySum(nums, k) == want ? " PASS\n" : " FAIL\n");
}

void testSplit(const string& label, const vector<int>& nums, bool want) {
    bool ok = canSplitAfterRemoval(nums) == want;
    ok = ok && canSplitAfterRemovalLinear(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testRemainder(const string& label, const vector<int>& nums, int mod, int k,
                   int want) {
    bool ok = shortestWithRemainder(nums, mod, k) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testMatrix(const string& label, const vector<vector<int>>& matrix, int k, int want) {
    cout << label << (countSubmatricesSumK(matrix, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("overlap", {1, 2, 1}, 3, 2);
    test("negative-zero-k", {1, -1, 0}, 0, 3);
    test("negative-k", {-1, 1, -1, -1}, -1, 5);
    test("none", {1, 2}, 5, 0);
    testSplit("split-remove-first", {7, 1, 1, 2}, true);
    testSplit("split-remove-last", {2, 1, 1, 7}, true);
    testSplit("split-remove-middle", {1, 5, 1}, true);
    testSplit("split-none", {1, 2, 4}, false);
    testSplit("split-too-short", {1, 1}, false);
    testRemainder("rem-latest-prefix", {3, 1}, 3, 1, 1);
    testRemainder("rem-whole-array", {1, 1, 1}, 5, 3, 3);
    testRemainder("rem-none", {5, 10}, 5, 1, -1);
    testMatrix("matrix-zero-sum", {{1, -1}, {-1, 1}}, 0, 5);
    testMatrix("matrix-row-and-cell", {{1, 2}, {3, 4}}, 3, 2);
    testMatrix("matrix-none", {{1, 1}}, 5, 0);
}

// follow-up: all values positive and k >= 1 -> sliding window, O(1) extra space; each
//   right end has at most one matching left end:
//     sum += nums[right];
//     while (sum > k) { sum -= nums[left]; left++; }
//     if (sum == k) { count++; }
// follow-up: longest subarray with sum k (LC 325; LC 525 maps 0 -> -1 and uses k = 0) ->
//   keep the FIRST index of each prefix, the empty prefix at index -1:
//     unordered_map<int, int> firstIndex{{0, -1}};
//     prefix += nums[i];
//     auto it = firstIndex.find(prefix - k);
//     if (it != firstIndex.end()) { best = max(best, i - it->second); }
//     firstIndex.insert({prefix, i});
//   insert never overwrites, so the first index stays; the subarray is it->second + 1..i
// follow-up: sum divisible by k (LC 974) -> key ((prefix % k) + k) % k, same counting
// follow-up: submatrices with sum k (LC 1074) -> countSubmatricesSumK above
