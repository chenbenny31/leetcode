// LC 698 Partition to K Equal Sum Subsets (Medium) — backtracking into k buckets
// Can the positive integers nums be split into k non-empty subsets with equal sums?
// Clarify: values positive; n small (<= 16); 1 <= k <= n; only yes / no, not the subsets.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::greater;

// nums is sorted descending; index = next number to place;
// buckets[b] = current sum of subset b, never above target
bool dfs(const vector<int>& nums, int index, vector<int>& buckets, int target) {
    if (index == (int)nums.size()) { return true; }
    int count = buckets.size();
    for (int bucket = 0; bucket < count; bucket++) {
        if (buckets[bucket] + nums[index] > target) { continue; }
        // an earlier bucket with the same sum was already tried: same search again
        bool tried = false;
        for (int earlier = 0; earlier < bucket; earlier++) {
            if (buckets[earlier] == buckets[bucket]) { tried = true; }
        }
        if (tried) { continue; }
        buckets[bucket] += nums[index];
        if (dfs(nums, index + 1, buckets, target)) { return true; }
        buckets[bucket] -= nums[index];
    }
    return false;
}

// Core idea: every bucket must reach sum / k; place the numbers largest first, each into
// any bucket it still fits, skipping buckets equal to one already tried.
// T: O(k^n) worst case, S: O(n + k).
bool canPartitionKSubsets(vector<int> nums, int k) {
    int total = 0;
    for (int num : nums) { total += num; }
    if (total % k != 0) { return false; }
    int target = total / k;
    // big numbers first: they fail fast
    sort(nums.begin(), nums.end(), greater<int>());
    if (nums[0] > target) { return false; }
    vector<int> buckets(k, 0);
    // all numbers placed with no bucket above target means every bucket equals target
    return dfs(nums, 0, buckets, target);
}

void test(const string& label, const vector<int>& nums, int k, bool want) {
    cout << label << (canPartitionKSubsets(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three-buckets", {1, 3, 2, 2, 3, 1}, 3, true);
    test("sum-not-divisible", {1, 2, 4}, 2, false);
    test("number-over-target", {5, 1, 1, 1}, 2, false);
    test("divisible-but-stuck", {3, 3, 3, 3}, 3, false);
    return 0;
}

// follow-up (ideas only):
// - matchsticks to square (LC 473) -> the same function with k = 4
// - fair distribution of cookies (LC 2305) -> same assignment into k buckets, but track
//   the largest bucket and keep the minimum over all assignments (prune when >= best)
// - n <= 16, need a guaranteed bound -> bitmask DP: dp[mask] = sum of the open bucket
//   (used sum % target) or -1 if unreachable; add any unused number that fits; O(n 2^n)
// - two subsets only (LC 416) -> 0/1 knapsack to total / 2, see
//   13-dp/07-partition-equal-subset-sum
