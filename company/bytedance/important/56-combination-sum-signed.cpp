// ByteDance phone (FT), LC 40 variant: candidates may hold negatives, zeros and
// positives. Return every distinct combination whose sum equals target:
// - each position is used at most once (equal values at different positions may both
//   be used);
// - selections with the same multiset of values count once;
// - each combination is non-decreasing; rows are in lexicographic order (a prefix
//   comes first);
// - when target == 0 the empty combination is included; other zero-sum combinations
//   are returned too.
// Return an empty array if there is no combination.
//   combinationSumSigned(candidates, target) -> vector<vector<int>>
// Constraints: n <= 16, |candidates[i]| <= 100.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// Backtracking over the sorted array. Every node of the search tree is a candidate
// combination (the path so far), so check the sum at every node, then keep extending:
// with negatives a longer combination can hit the target again, so there is no
// "sum too big, stop" pruning as in LC 40. Skipping a value equal to the previous one
// at the same depth makes each multiset appear once. Visiting children in ascending
// order and recording a path before its extensions gives lexicographic order.
void dfs(const vector<int>& nums, int start, int sum, int target, vector<int>& path,
         vector<vector<int>>& result) {
    if (sum == target) {
        result.push_back(path);
    }
    for (int i = start; i < (int)nums.size(); i++) {
        // same value at the same depth would repeat a multiset
        if (i > start && nums[i] == nums[i - 1]) {
            continue;
        }
        path.push_back(nums[i]);
        dfs(nums, i + 1, sum + nums[i], target, path, result);
        path.pop_back();
    }
}

// T: O(2^n * n) (every subset may be visited and copied), S: O(n) besides the output.
vector<vector<int>> combinationSumSigned(vector<int> candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> result;
    vector<int> path;
    dfs(candidates, 0, 0, target, path, result);
    return result;
}

void test(const string& label, const vector<int>& candidates, int target,
          const vector<vector<int>>& want) {
    bool ok = combinationSumSigned(candidates, target) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("negative-needed", {5, -2, 3}, 1, {{-2, 3}});
    test("duplicates-once", {2, 2, 2}, 4, {{2, 2}});
    test("zero-target", {-2, 2, 0}, 0, {{}, {-2, 0, 2}, {-2, 2}, {0}});
    test("none", {1, 3}, 2, {});
    test("empty-input", {}, 0, {{}});
}
