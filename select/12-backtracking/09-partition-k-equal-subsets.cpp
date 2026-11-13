// LC 698 Partition to K Equal Sum Subsets (Medium) — backtracking into k buckets
// return if positive nums split into k non-empty subsets w/ equal sums
//
// clarify:
// values positive -> bucket sums only grow, prune at target
// n small (<= 16) -> exponential search ok
// 1 <= k <= n -> no divide by 0
// only yes / no -> stop at first success

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

// buckets[b] = sum of subset b, never above target; index = next num to place
bool dfs(const vector<int>& nums, int index, vector<int>& buckets, int target) {
    if (index == (int)nums.size()) { return true; }
    int count = buckets.size();
    for (int bucket = 0; bucket < count; bucket++) {
        if (buckets[bucket] + nums[index] > target) { continue; }
        // an earlier bucket w/ same sum was already tried: same search again
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

// core idea: each bucket must reach sum / k; place nums largest first into any that fits
// prune: skip a bucket w/ same sum as an earlier one
// T: O(k^n), S: O(n + k) — each num tries up to k buckets
bool canPartitionKSubsets(vector<int> nums, int k) {
    int total = 0;
    for (int num : nums) { total += num; }
    if (total % k != 0) { return false; }
    int target = total / k;
    // big nums first: they fail fast
    sort(nums.begin(), nums.end(), greater<int>());
    if (nums[0] > target) { return false; }
    vector<int> buckets(k, 0);
    // all placed w/ no bucket above target = every bucket equals target
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

// follow-up:
// matchsticks to square (LC 473) -> same function w/ k = 4
// fair distribution of cookies (LC 2305) -> same buckets; track max bucket, keep the min
// n <= 16, guaranteed bound -> bitmask DP: dp[mask] = sum of open bucket, O(n * 2^n)
// two subsets only (LC 416) -> 0/1 knapsack to total / 2 (13-dp/07)
