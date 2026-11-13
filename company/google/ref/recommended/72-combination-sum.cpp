// 72 · Combination Sum — LC 39
// Mother: select/12-backtracking/03-combination-sum.cpp
// distinct positive candidates, each reusable; return all unique combos summing to target
// reported: 1s and 2s summing to N ×4
//
// clarify:
// [2, 3] vs [3, 2] -> same combo (multiset), list once
// no combo -> empty result
// candidate reuse -> any num of times

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// path = chosen values; remain = target - sum(path); start = first idx allowed
void dfs(const vector<int>& nums, int start, int remain, vector<int>& path,
         vector<vector<int>>& result) {
    if (remain == 0) {
        result.push_back(path);
        return;
    }
    int n = nums.size();
    for (int i = start; i < n; i++) {
        // sorted -> every later candidate too big as well
        if (nums[i] > remain) { break; }
        path.push_back(nums[i]);
        // i, not i + 1: same candidate may be reused
        dfs(nums, i, remain - nums[i], path, result);
        path.pop_back();
    }
}

// core idea: sort, backtrack on remain; recurse on i (reuse), never go back (no dup)
// T: O(n^(t/m)), S: O(t/m) + output — t = target, m = min candidate
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

// follow-up:
// strings of '1' / '2' w/ digit sum n (Google) -> dfs appends '1', '2' if remain >= 2
// cnt of those strings -> Fibonacci(n + 1), like climbing stairs
// each candidate once, w/ dup (LC 40) -> recurse on i + 1, skip dup when i > start
// cnt only (LC 518) -> coin DP, dp[s] += dp[s - c], coins in outer loop (69)
