// LC 698 Partition to K Equal Sum Subsets: can nums be split into k non-empty subsets
// with equal sums, every element used exactly once?
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::greater;

bool dfs(const vector<int>& nums, int i, int target, vector<int>& buckets) {
    int n = nums.size();
    if (i == n) { return true; }
    int k = buckets.size();
    for (int j = 0; j < k; j++) {
        // sum budget: a bucket may never exceed target
        if (buckets[j] + nums[i] > target) { continue; }
        buckets[j] += nums[i];
        if (dfs(nums, i + 1, target, buckets)) { return true; }
        buckets[j] -= nums[i];
        // all empty buckets are alike: failing on one means failing on every other
        if (buckets[j] == 0) { break; }
    }
    return false;
}

// Backtracking: place each number, largest first, into a bucket that stays within the
// target sum. buckets[j] = sum of the numbers currently in bucket j.
// T: O(k^n) worst case, S: O(n + k).
bool canPartitionKSubsets(vector<int>& nums, int k) {
    int sum = 0;
    for (int x : nums) { sum += x; }
    if (sum % k != 0) { return false; }
    int target = sum / k;
    sort(nums.begin(), nums.end(), greater<int>());
    if (nums[0] > target) { return false; }
    vector<int> buckets(k, 0);
    return dfs(nums, 0, target, buckets);
}

void test(const string& label, vector<int> nums, int k, bool want) {
    cout << label << (canPartitionKSubsets(nums, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three-ways", {6, 1, 5, 2, 4, 3}, 3, true);
    test("sum-not-divisible", {1, 2, 4}, 2, false);
    test("element-too-big", {1, 1, 5}, 3, false);
    test("sum-fits-no-split", {5, 5, 2, 2, 2}, 2, false);
}
