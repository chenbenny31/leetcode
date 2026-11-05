// ByteDance phone (FT), LC 40 variant: candidates may be negative, zero or positive.
// Return every distinct combination (each position used once, equal multisets once) that
// sums to target; each non-decreasing, rows lexicographic; target 0 includes [].

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// every path is a candidate: record it on a hit, then keep extending
void dfs(const vector<int>& nums, int start, int sum, int target, vector<int>& path,
         vector<vector<int>>& result) {
    // no return after a hit: with negatives a longer path can hit target again
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

// Sorted backtracking, no "sum too big" pruning (negatives); a path is recorded before
// its extensions, so rows come out lexicographic. T: O(2^n * n), S: O(n) besides output.
vector<vector<int>> combinationSumSigned(vector<int> candidates, int target) {
    sort(candidates.begin(), candidates.end());
    // combinations found, in lexicographic order
    vector<vector<int>> result;
    // current combination
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

// follow-up: prune with negatives -> extra sum reachable from start is
//   [negSuffix[start], posSuffix[start]]; return if target - sum is outside
// follow-up: count distinct multisets -> group equal values (v, cnt); dp over sums
//   choosing 0..cnt copies per group; sums in [-1600, 1600], O(n * 3201)
// follow-up: existence for larger n -> meet in the middle: 2^(n/2) sums per half, sort
//   one, binary search target - s; O(2^(n/2) * n)
// follow-up: unlimited reuse (LC 39) with negatives -> infinitely many answers
//   (-1 + 1 repeats), so a length cap is needed
