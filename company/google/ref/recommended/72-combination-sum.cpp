// 72 · Combination Sum — LC 39
// Mother problem: select/12-backtracking/03-combination-sum.cpp
//
// LC 39 (Medium), backtracking with reuse: distinct positive candidates; return all
// unique combinations summing to target, each candidate usable any number of times.
// Google reports: 1s and 2s summing to N ×4
//
// Clarify
// - combinations unique as multisets ([2, 3] == [3, 2]) → recurse from i, never go back
// - no combination → empty result
//
// Core idea
// - Sort, backtrack with remain = target - sum; recurse on i (not i + 1) to allow reuse.
// - Break when nums[i] > remain (sorted, so later ones are too big).
// - T O(n^(t/m)), t = target, m = smallest candidate; S O(t/m) besides the output.
//
// Tests: reuse, ones-or-five, none
//
// Follow-ups
// - each candidate once, with duplicates (LC 40) → recurse on i + 1, skip
//   nums[i] == nums[i-1] when i > start
// - strings of '1' / '2' with digit sum n → dfs appends '1', then '2' if remain >= 2,
//   records at remain 0 ("" for n = 0); count is Fibonacci(n + 1) like climbing stairs
// - count only (LC 518) → coin DP, dp[s] += dp[s - c] with coins outer (69)
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// path = chosen values; remain = target - sum(path); start = first index allowed
void dfs(const vector<int>& nums, int start, int remain, vector<int>& path,
         vector<vector<int>>& result) {
    if (remain == 0) {
        result.push_back(path);
        return;
    }
    int n = nums.size();
    for (int i = start; i < n; i++) {
        // sorted: every later candidate is too big as well
        if (nums[i] > remain) { break; }
        path.push_back(nums[i]);
        // i, not i + 1: the same candidate may be reused
        dfs(nums, i, remain - nums[i], path, result);
        path.pop_back();
    }
}

// Sort, then backtrack with pruning. T: O(n^(t/m)) for target t and smallest
// candidate m (max depth t/m), S: O(t/m) + output.
vector<vector<int>> combinationSum(vector<int> candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> result;
    vector<int> path;
    dfs(candidates, 0, target, path, result);
    return result;
}

void test(const string& label, const vector<int>& candidates, int target,
          const vector<vector<int>>& want) {
    bool ok = combinationSum(candidates, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("reuse", {3, 2}, 6, {{2, 2, 2}, {3, 3}});
    test("ones-or-five", {5, 1}, 5, {{1, 1, 1, 1, 1}, {5}});
    test("none", {4}, 3, {});
}
