// 70 · Subsets — LC 78, 90, 22
// Mother: select/12-backtracking/01-subsets.cpp
// return all subsets (the power set) of nums; values are distinct
// reported: LC 22, LC 90, subsequences are words, subsets with / without dups ×2
//
// clarify:
// empty input -> {{}}
// output order -> any
// n -> 2^n subsets, so n is small
// dups in nums -> follow-up (LC 90)

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// path = chosen values so far; only nums[start..] may still be added
void dfs(const vector<int>& nums, int start, vector<int>& path,
         vector<vector<int>>& result) {
    result.push_back(path);
    int n = nums.size();
    for (int i = start; i < n; i++) {
        path.push_back(nums[i]);
        dfs(nums, i + 1, path, result);
        path.pop_back();
    }
}

// core idea: backtracking where every node of the tree is an answer
// record the path, then add each nums[start..] in turn, recurse on i + 1, pop
// T: O(n * 2^n), S: O(n) + output — 2^n subsets, O(n) to copy each
vector<vector<int>> subsets(const vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(nums, 0, path, result);
    return result;
}

void test(const string& label, const vector<int>& nums, const vector<vector<int>>& want) {
    cout << label << (subsets(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two", {1, 2}, {{}, {1}, {1, 2}, {2}});
    test("three-unsorted", {3, 1, 2},
         {{}, {3}, {3, 1}, {3, 1, 2}, {3, 2}, {1}, {1, 2}, {2}});
    test("empty", {}, {{}});
}

// follow-up:
// with dups (LC 90) -> sort, skip nums[i] == nums[i-1] when i > start
// generate parentheses (LC 22) -> add '(' while open < n, ')' while close < open
// iterative -> bitmask over [0, 2^n), take nums[i] if bit i is set
