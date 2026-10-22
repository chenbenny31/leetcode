// LC 46 Permutations: given distinct integers, return all their permutations in any
// order.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

void dfs(const vector<int>& nums, vector<bool>& used, vector<int>& cur,
         vector<vector<int>>& out) {
    int n = nums.size();
    if ((int)cur.size() == n) {
        out.push_back(cur);
        return;
    }
    for (int i = 0; i < n; i++) {
        if (used[i]) { continue; }
        used[i] = true;
        cur.push_back(nums[i]);
        dfs(nums, used, cur, out);
        cur.pop_back();
        used[i] = false;
    }
}

// Backtracking: each level picks any unused element. used[i] = nums[i] is in cur.
// T: O(n * n!), S: O(n).
vector<vector<int>> permute(vector<int>& nums) {
    vector<vector<int>> out;
    vector<bool> used(nums.size(), false);
    vector<int> cur;
    dfs(nums, used, cur, out);
    return out;
}

void test(const string& label, vector<int> nums, vector<vector<int>> want) {
    vector<vector<int>> got = permute(nums);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("three", {1, 2, 3},
         {{1, 2, 3}, {1, 3, 2}, {2, 1, 3}, {2, 3, 1}, {3, 1, 2}, {3, 2, 1}});
    test("single", {7}, {{7}});
    test("empty", {}, {{}});
}
