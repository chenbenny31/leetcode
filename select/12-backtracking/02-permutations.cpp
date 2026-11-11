// LC 46 Permutations (Medium). Pattern: backtracking with a used[] array.
// Return all permutations of distinct integers nums, in any order.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::sort;
using std::reverse;
using std::swap;

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

// nums is sorted; path = permutation prefix; used[i] = nums[i] is already in path
void dfsUnique(const vector<int>& nums, vector<bool>& used, vector<int>& path,
               vector<vector<int>>& result) {
    int n = nums.size();
    if ((int)path.size() == n) {
        result.push_back(path);
        return;
    }
    for (int i = 0; i < n; i++) {
        if (used[i]) { continue; }
        // equal values are placed left to right only, so each arrangement appears once
        if (i > 0 && nums[i] == nums[i - 1] && !used[i - 1]) { continue; }
        used[i] = true;
        path.push_back(nums[i]);
        dfsUnique(nums, used, path, result);
        path.pop_back();
        used[i] = false;
    }
}

// LC 47: sort, then permute with a skip for equal values. T: O(n * n!), S: O(n) + output.
vector<vector<int>> permuteUnique(vector<int> nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> result;
    vector<bool> used(nums.size(), false);
    vector<int> path;
    dfsUnique(nums, used, path, result);
    return result;
}

// LC 31: find the rightmost ascent nums[pivot] < nums[pivot + 1], swap nums[pivot] with
// the rightmost larger value, reverse the (descending) suffix. T: O(n), S: O(1).
void nextPermutation(vector<int>& nums) {
    int n = nums.size();
    int pivot = n - 2;
    while (pivot >= 0 && nums[pivot] >= nums[pivot + 1]) { pivot--; }
    // pivot == -1: already the last permutation, the reverse below wraps to the first
    if (pivot >= 0) {
        // suffix is non-increasing, so the rightmost larger value is the smallest one
        int swapWith = n - 1;
        while (nums[swapWith] <= nums[pivot]) { swapWith--; }
        swap(nums[pivot], nums[swapWith]);
    }
    reverse(nums.begin() + pivot + 1, nums.end());
}

void test(const string& label, const vector<int>& nums, const vector<vector<int>>& want) {
    vector<vector<int>> got = permute(nums);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

void testUnique(const string& label, const vector<int>& nums,
                const vector<vector<int>>& want) {
    bool ok = permuteUnique(nums) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testNext(const string& label, vector<int> nums, const vector<int>& want) {
    nextPermutation(nums);
    cout << label << (nums == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two", {2, 1}, {{1, 2}, {2, 1}});
    test("three", {3, 1, 2},
         {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}});
    test("single", {7}, {{7}});
    testUnique("one-pair", {2, 1, 1}, {{1, 1, 2}, {1, 2, 1}, {2, 1, 1}});
    testUnique("two-pairs", {2, 1, 2, 1},
               {{1, 1, 2, 2}, {1, 2, 1, 2}, {1, 2, 2, 1},
                {2, 1, 1, 2}, {2, 1, 2, 1}, {2, 2, 1, 1}});
    testUnique("all-same", {5, 5, 5}, {{5, 5, 5}});
    testNext("tail-ascent", {1, 2, 3}, {1, 3, 2});
    testNext("dup-swap", {1, 2, 2, 1}, {2, 1, 1, 2});
    testNext("last-wraps", {3, 2, 1}, {1, 2, 3});
}
