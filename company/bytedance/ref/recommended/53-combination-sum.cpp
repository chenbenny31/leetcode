// ByteDance (hack2hire) / LC 39: distinct positive candidates; return all unique
// combinations summing to target, each candidate reusable any number of times.
// Follow-up LC 40: candidates may repeat and each element is used at most once.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// path = chosen candidates; remain = target - sum(path); start = first index allowed
void dfs(const vector<int>& nums, int start, int remain, vector<int>& path,
         vector<vector<int>>& result) {
    if (remain == 0) {
        result.push_back(path);
        return;
    }
    for (int i = start; i < (int)nums.size(); i++) {
        // sorted: every later candidate is too big as well
        if (nums[i] > remain) {
            break;
        }
        path.push_back(nums[i]);
        // i, not i + 1: reuse allowed
        dfs(nums, i, remain - nums[i], path, result);
        path.pop_back();
    }
}

// Sort, then backtrack with pruning. T: O(n^(t/m)), S: O(t/m);
// t = target, m = smallest candidate (max depth t/m).
vector<vector<int>> combinationSum(vector<int> candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> result;
    vector<int> path;
    dfs(candidates, 0, target, path, result);
    return result;
}

void dfsOnce(const vector<int>& nums, int start, int remain, vector<int>& path,
             vector<vector<int>>& result) {
    if (remain == 0) {
        result.push_back(path);
        return;
    }
    for (int i = start; i < (int)nums.size(); i++) {
        if (nums[i] > remain) {
            break;
        }
        // same value at the same depth would repeat a combination
        if (i > start && nums[i] == nums[i - 1]) {
            continue;
        }
        path.push_back(nums[i]);
        dfsOnce(nums, i + 1, remain - nums[i], path, result);
        path.pop_back();
    }
}

// Follow-up: recurse with i + 1, skip equal siblings. T: O(2^n * n), S: O(n).
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
    test("none", {2}, 1, {});
    testOnce("once-dup", {2, 1, 1}, 3, {{1, 2}});
    testOnce("once-both", {1, 2, 1}, 2, {{1, 1}, {2}});
}

// follow-up: once each, dup candidates (LC 40) -> see combinationSum2() above; i + 1
//   and skip nums[i] == nums[i - 1] when i > start
// follow-up: count only (LC 518) -> dp[s] += dp[s - c], coins in outer loop, O(n * t)
// follow-up: order matters (LC 377) -> same dp with target sum in the outer loop
// follow-up: k numbers from 1..9 (LC 216) -> dfs over 1..9 with i + 1, stop at size k
