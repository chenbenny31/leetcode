// LC 46 Permutations (Medium) — backtracking with a used[] array
// Return all permutations of the distinct integers nums, in any order.
// Clarify: values distinct; output order; empty input; n small enough for n!.

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

// Core idea: backtracking; each level appends any value not yet used, and a full-length
// path is one permutation. T: O(n * n!), S: O(n) + output.
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

// follow-up (ideas only):
// - duplicates in nums (LC 47) -> sort, and skip nums[i] when it equals nums[i - 1] and
//   used[i - 1] is false, so equal values are placed left to right only
// - next permutation in place (LC 31) -> find the rightmost ascent nums[pivot] <
//   nums[pivot + 1], swap nums[pivot] with the rightmost larger value, reverse the
//   suffix; no ascent means reverse everything (wraps to the first)
// - no used[] array -> swap nums[i] into position start, recurse on start + 1, swap back
