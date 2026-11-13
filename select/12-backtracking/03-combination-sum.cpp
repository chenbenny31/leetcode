// LC 39 Combination Sum (Medium) — backtracking with reuse (recurse on i, not i + 1)
// Distinct positive candidates; return all unique combinations summing to target, each
// candidate usable any number of times.
// Clarify: candidates positive and distinct; reuse allowed; order inside a combination
//   does not matter; no combination returns an empty list.

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

// Core idea: sort, then backtrack choosing candidates in non-decreasing index order (so
// each combination appears once), cutting a branch when a candidate exceeds remain.
// T: O(n^(t/m)) for target t and smallest candidate m, S: O(t/m) + output.
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

// follow-up (ideas only):
// - each element used once, input has duplicates (LC 40) -> recurse on i + 1, and at
//   one depth skip a value equal to the previous one (i > start)
// - all strings over {'1', '2'} with digit sum n, in lexicographic order (Google) ->
//   dfs(remain): append '1', then '2' if remain >= 2; count is Fibonacci(n + 1)
// - count the combinations only (LC 518) -> dp[sum] += dp[sum - coin] with coins in the
//   outer loop
