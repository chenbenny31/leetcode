// LC 78 Subsets (Medium) — backtracking (every tree node is an answer) / bitmask
// return all subsets (power set) of distinct nums, any order
//
// clarify:
// values distinct -> no dedup; dups -> follow-up (LC 90)
// output order -> any
// empty set -> included
// n small -> 2^n subsets fit

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// path = chosen so far; only nums[start..] may still be added
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

// solution 1: backtracking; extends to dups / pruning / size-k variants
// core idea: every tree node is a subset; child only adds a value right of the last
// T: O(n * 2^n), S: O(n) + output
vector<vector<int>> subsets(const vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(nums, 0, path, result);
    return result;
}

// solution 2: bitmask; iterative, use it when n <= ~20
// core idea: each mask in [0, 2^n) is one subset; bit i set = nums[i] taken
// T: O(n * 2^n), S: O(1) + output
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

// follow-up:
// dups in nums (LC 90) -> sort; skip if i > start && nums[i] == nums[i - 1]
// generate parentheses (LC 22) -> add '(' while open < n, ')' while close < open
// subsets of size k (LC 77) -> record only when path.size() == k; prune if too few left
// phone letter combos (LC 17) -> one choice per digit; record at path len = digits len
// palindrome partitioning (LC 131) -> choose next cut end; recurse only if palindrome
