// LC 39 Combination Sum (Medium). Pattern: backtracking with reuse (recurse on i).
// Distinct positive candidates; return all unique combinations summing to target, each
// candidate usable any number of times.

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

// nums is sorted; same state as dfs, but each index is used at most once
void dfsOnce(const vector<int>& nums, int start, int remain, vector<int>& path,
             vector<vector<int>>& result) {
    if (remain == 0) {
        result.push_back(path);
        return;
    }
    int n = nums.size();
    for (int i = start; i < n; i++) {
        if (nums[i] > remain) { break; }
        // equal value already tried at this depth: it would repeat a combination
        if (i > start && nums[i] == nums[i - 1]) { continue; }
        path.push_back(nums[i]);
        // i + 1: each element is used at most once
        dfsOnce(nums, i + 1, remain - nums[i], path, result);
        path.pop_back();
    }
}

// LC 40: sort, backtrack on i + 1, skip equal values at one depth.
// T: O(n * 2^n), S: O(n) + output.
vector<vector<int>> combinationSum2(vector<int> candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> result;
    vector<int> path;
    dfsOnce(candidates, 0, target, path, result);
    return result;
}

void test(const string& label, const vector<int>& candidates, int target,
          const vector<vector<int>>& want) {
    bool ok = combinationSum(candidates, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testOnce(const string& label, const vector<int>& candidates, int target,
              const vector<vector<int>>& want) {
    bool ok = combinationSum2(candidates, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("reuse", {3, 2}, 6, {{2, 2, 2}, {3, 3}});
    test("ones-or-five", {5, 1}, 5, {{1, 1, 1, 1, 1}, {5}});
    test("none", {4}, 3, {});
    testOnce("dup-pairs", {2, 1, 2, 1}, 4, {{1, 1, 2}, {2, 2}});
    testOnce("one-each", {3, 1, 3}, 4, {{1, 3}});
    testOnce("no-reuse", {2}, 4, {});
}

// follow-up: count only (LC 518) -> dp[s] += dp[s - c] with coins in the outer loop
