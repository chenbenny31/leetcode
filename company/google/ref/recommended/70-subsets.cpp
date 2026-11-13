// 70 · Subsets — LC 78, 90, 22
// Mother problem: select/12-backtracking/01-subsets.cpp
//
// LC 78 (Medium), backtracking where every node of the tree is an answer: return all
// subsets (the power set) of nums; values are distinct.
// Google reports: generate parentheses, LC 90, subsequences are words, subsets with /
// without duplicates ×2
//
// Clarify
// - empty input → {{}}; output order any (the code emits DFS order, prefix first)
// - n up to ~10 → 2^n subsets is fine
//
// Core idea
// - Push the path, then try nums[start..], recurse on i + 1, pop.
// - T O(n * 2^n), S O(n) besides the output.
//
// Tests: two values, three unsorted, empty
//
// Follow-ups
// - with duplicates (LC 90) → sort, skip nums[i] == nums[i-1] when i > start
// - generate parentheses (LC 22) → dfs adds '(' while open < n and ')' while
//   close < open, records the path when close == n
// - iterative → bitmask over [0, 2^n), take nums[i] if (mask >> i) & 1
//
//////////////////////////////////////////////////////////////////////////////////////////

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

// Backtracking: 2^n subsets, copying each is O(n). T: O(n * 2^n), S: O(n) + output.
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
