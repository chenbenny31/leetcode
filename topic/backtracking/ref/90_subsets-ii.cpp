// LC 90 Subsets II: given integers that may repeat, return every unique subset in any
// order.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

void dfs(const vector<int>& nums, int start, vector<int>& cur,
         vector<vector<int>>& out) {
    out.push_back(cur);
    int n = nums.size();
    for (int i = start; i < n; i++) {
        // equal to the previous sibling: that branch already covered this value here
        if (i > start && nums[i] == nums[i - 1]) { continue; }
        cur.push_back(nums[i]);
        dfs(nums, i + 1, cur, out);
        cur.pop_back();
    }
}

// Backtracking on a sorted array: every node of the recursion is a subset, and skipping
// equal siblings removes duplicates. cur is the partial subset. T: O(n * 2^n), S: O(n).
vector<vector<int>> subsetsWithDup(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> out;
    vector<int> cur;
    dfs(nums, 0, cur, out);
    return out;
}

void test(const string& label, vector<int> nums, vector<vector<int>> want) {
    vector<vector<int>> got = subsetsWithDup(nums);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("duplicate", {2, 1, 2}, {{}, {1}, {1, 2}, {1, 2, 2}, {2}, {2, 2}});
    test("all-equal", {4, 4}, {{}, {4}, {4, 4}});
    test("empty", {}, {{}});
}
