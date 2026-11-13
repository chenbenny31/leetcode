// LC 46 Permutations (Medium) — backtracking with a used[] array
// return all permutations of distinct nums, any order
//
// clarify:
// values distinct -> no dedup; dups -> follow-up (LC 47)
// output order -> any
// empty input -> ask; here {{}}
// n small -> n! results fit

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// path = permutation prefix; used[i] = nums[i] already in path
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

// core idea: each level appends any unused value; full-length path = one permutation
// T: O(n * n!), S: O(n) + output
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

// follow-up:
// dups in nums (LC 47) -> sort; skip if nums[i] == nums[i - 1] && !used[i - 1]
// next permutation (LC 31) -> rightmost ascent, swap w/ rightmost larger, reverse suffix
// no used[] -> swap nums[i] into position start, recurse on start + 1, swap back
