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

// path = digits chosen so far; remain = n - digit sum of path
void dfsOneTwo(int remain, string& path, vector<string>& result) {
    if (remain == 0) {
        result.push_back(path);
        return;
    }
    // '1' before '2' keeps the output in lexicographic order
    path.push_back('1');
    dfsOneTwo(remain - 1, path, result);
    path.pop_back();
    if (remain >= 2) {
        path.push_back('2');
        dfsOneTwo(remain - 2, path, result);
        path.pop_back();
    }
}

// Google: all strings over {'1', '2'} with digit sum n (n >= 0), in lexicographic order;
// n = 0 -> {""}. Count is Fibonacci(n + 1) (F(1) = F(2) = 1), like climbing stairs.
// T: O(n * Fib(n + 1)), S: O(n) + output.
vector<string> oneTwoStrings(int n) {
    vector<string> result;
    string path;
    dfsOneTwo(n, path, result);
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

void testOneTwo(const string& label, int n, const vector<string>& want) {
    cout << label << (oneTwoStrings(n) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("reuse", {3, 2}, 6, {{2, 2, 2}, {3, 3}});
    test("ones-or-five", {5, 1}, 5, {{1, 1, 1, 1, 1}, {5}});
    test("none", {4}, 3, {});
    testOnce("dup-pairs", {2, 1, 2, 1}, 4, {{1, 1, 2}, {2, 2}});
    testOnce("one-each", {3, 1, 3}, 4, {{1, 3}});
    testOnce("no-reuse", {2}, 4, {});
    testOneTwo("onetwo-zero", 0, {""});
    testOneTwo("onetwo-three", 3, {"111", "12", "21"});
    testOneTwo("onetwo-four", 4, {"1111", "112", "121", "211", "22"});
}

// follow-up: count only (LC 518) -> dp[s] += dp[s - c] with coins in the outer loop
