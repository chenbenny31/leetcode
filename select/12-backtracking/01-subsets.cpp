// LC 78 Subsets (Medium) — backtracking (every tree node is an answer) / bitmask
// Return all subsets (the power set) of nums, in any order; values are distinct.
// Clarify: values distinct; output order; empty set included; n small enough for 2^n.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

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

// Core idea: backtracking; every node of the tree is a subset, and a child only adds a
// value to the right of the last one, so no subset repeats.
// T: O(n * 2^n), S: O(n) + output.
vector<vector<int>> subsets(const vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(nums, 0, path, result);
    return result;
}

// Core idea: each mask in [0, 2^n) is one subset: bit i set means nums[i] is taken.
// T: O(n * 2^n), S: O(1) + output.
vector<vector<int>> subsetsBitmask(const vector<int>& nums) {
    int n = nums.size();
    vector<vector<int>> result;
    for (int mask = 0; mask < (1 << n); mask++) {
        vector<int> subset;
        for (int i = 0; i < n; i++) {
            if ((mask >> i) & 1) { subset.push_back(nums[i]); }
        }
        result.push_back(subset);
    }
    return result;
}

void test(const string& label, const vector<int>& nums, vector<vector<int>> want) {
    vector<vector<int>> byDfs = subsets(nums);
    vector<vector<int>> byMask = subsetsBitmask(nums);
    sort(byDfs.begin(), byDfs.end());
    sort(byMask.begin(), byMask.end());
    sort(want.begin(), want.end());
    bool ok = byDfs == want && byMask == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two", {1, 2}, {{}, {1}, {1, 2}, {2}});
    test("three-unsorted", {3, 1, 2},
         {{}, {3}, {3, 1}, {3, 1, 2}, {3, 2}, {1}, {1, 2}, {2}});
    test("empty", {}, {{}});
}

// follow-up (ideas only):
// - duplicates in nums (LC 90) -> sort, and at one depth skip a value equal to the
//   previous one (i > start && nums[i] == nums[i - 1])
// - generate parentheses (LC 22) -> same path DFS: add '(' while open < n, ')' while
//   close < open; record when close == n
// - subsets of size k only (LC 77) -> record only when path.size() == k; stop early when
//   the remaining values cannot fill the path
// - letter combinations of a phone number (LC 17) -> one choice per digit position:
//   try each letter of digits[i], record when the path is as long as digits
// - palindrome partitioning (LC 131) -> choose the next cut end; recurse only when
//   s[start..end] is a palindrome, record when start reaches the end
