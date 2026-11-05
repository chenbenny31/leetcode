// ByteDance (hack2hire) / LC 78: all subsets of distinct nums (the power set).
// Follow-up LC 90: nums may contain duplicates; each distinct subset appears once.

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::sort;

// path = chosen elements so far; every node of the recursion tree is a subset
void dfs(const vector<int>& nums, int start, vector<int>& path,
         vector<vector<int>>& result) {
    result.push_back(path);
    for (int i = start; i < (int)nums.size(); i++) {
        path.push_back(nums[i]);
        dfs(nums, i + 1, path, result);
        path.pop_back();
    }
}

// Backtracking: 2^n subsets, copying each is O(n). T: O(2^n * n), S: O(n) + output.
vector<vector<int>> subsets(const vector<int>& nums) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(nums, 0, path, result);
    return result;
}

void dfsDup(const vector<int>& nums, int start, vector<int>& path,
            vector<vector<int>>& result) {
    result.push_back(path);
    for (int i = start; i < (int)nums.size(); i++) {
        // same value as the previous sibling at this depth: already explored
        if (i > start && nums[i] == nums[i - 1]) {
            continue;
        }
        path.push_back(nums[i]);
        dfsDup(nums, i + 1, path, result);
        path.pop_back();
    }
}

// Follow-up: sort, then skip equal siblings. T: O(2^n * n), S: O(n) + output.
vector<vector<int>> subsetsWithDup(vector<int> nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> result;
    vector<int> path;
    dfsDup(nums, 0, path, result);
    return result;
}

void test(const string& label, const vector<int>& nums, const vector<vector<int>>& want) {
    cout << label << (subsets(nums) == want ? " PASS\n" : " FAIL\n");
}

void testDup(const string& label, const vector<int>& nums,
             const vector<vector<int>>& want) {
    cout << label << (subsetsWithDup(nums) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three", {1, 2, 3}, {{}, {1}, {1, 2}, {1, 2, 3}, {1, 3}, {2}, {2, 3}, {3}});
    test("empty", {}, {{}});
    testDup("dup", {2, 1, 2}, {{}, {1}, {1, 2}, {1, 2, 2}, {2}, {2, 2}});
}

// follow-up: duplicates (LC 90) -> see subsetsWithDup() above; sort, skip
//   nums[i] == nums[i - 1] when i > start
// follow-up: iterative / bitmask -> mask in [0, 2^n), take nums[i] if mask >> i & 1,
//   O(2^n * n)
// follow-up: only size k (LC 77) -> record when path.size() == k; prune when
//   n - i < k - path.size()
