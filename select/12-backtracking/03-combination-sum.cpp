// LC 39 Combination Sum (Medium) — backtracking with reuse (recurse on i, not i + 1)
// distinct positive candidates; return all unique combos summing to target, reuse ok
//
// clarify:
// candidates positive -> remain shrinks, recursion ends
// candidates distinct -> no dedup; dups -> follow-up (LC 40)
// reuse allowed -> recurse on i, not i + 1
// order inside a combo does not matter -> only pick idx >= start
// no combo -> return {}

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
        // sorted: later candidates are too big too
        if (nums[i] > remain) { break; }
        path.push_back(nums[i]);
        // i, not i + 1: same candidate may be reused
        dfs(nums, i, remain - nums[i], path, result);
        path.pop_back();
    }
}

// core idea: sort; pick candidates in non-decreasing idx order; cut when one > remain
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
// each used once, input has dups (LC 40) -> recurse on i + 1; skip dup if i > start
// strings over {'1', '2'} w/ digit sum n (Google) -> dfs(remain): '1', then '2' if >= 2
// cnt combos only (LC 518) -> dp[sum] += dp[sum - coin], coins in outer loop
