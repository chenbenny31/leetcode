// 71 · Permutations — LC 46, 47
// Mother problem: select/12-backtracking/02-permutations.cpp
//
// LC 46 (Medium), backtracking with a used[] array: return all permutations of distinct
// integers nums, in any order.
// Google reports: permutations II (1p3a)
//
// Clarify
// - output order any; n up to ~8 → n! results is fine; single value → itself
//
// Core idea
// - Each level picks any unused value; record the path when it has n values.
// - T O(n * n!), S O(n) besides the output.
//
// Tests: two values, three values, single value
//
// Follow-ups
// - with duplicates (LC 47) → sort, skip nums[i] == nums[i-1] when !used[i-1] (equal
//   values are placed left to right only)
// - next permutation (LC 31) → rightmost ascent nums[p] < nums[p+1], swap nums[p] with
//   the rightmost larger value, reverse the suffix after p (no ascent: reverse all)
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

// path = permutation prefix; used[i] = nums[i] is already in path
void dfs(const vector<int>& nums, vector<bool>& used, vector<int>& path,
         vector<vector<int>>& result) {
    int n = nums.size();
    if ((int)path.size() == n) {
        result.push_back(path);
        return;
    }
    for (int i = 0; i < n; i++) {
        if (used[i]) { continue; }
        used[i] = true;
        path.push_back(nums[i]);
        dfs(nums, used, path, result);
        path.pop_back();
        used[i] = false;
    }
}

// Each level picks any unused value. T: O(n * n!), S: O(n) + output.
vector<vector<int>> permute(const vector<int>& nums) {
    vector<vector<int>> result;
    vector<bool> used(nums.size(), false);
    vector<int> path;
    dfs(nums, used, path, result);
    return result;
}

void test(const string& label, const vector<int>& nums, const vector<vector<int>>& want) {
    vector<vector<int>> got = permute(nums);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two", {2, 1}, {{1, 2}, {2, 1}});
    test("three", {3, 1, 2},
         {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}});
    test("single", {7}, {{7}});
}
