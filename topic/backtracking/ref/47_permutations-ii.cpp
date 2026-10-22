// LC 47 Permutations II: given integers that may repeat, return all unique permutations
// in any order.
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
        // equal twin not placed yet: use twins left to right so each order appears once
        if (i > 0 && nums[i] == nums[i - 1] && !used[i - 1]) { continue; }
        used[i] = true;
        cur.push_back(nums[i]);
        dfs(nums, used, cur, out);
        cur.pop_back();
        used[i] = false;
    }
}

// Backtracking on a sorted array with a used array and the twin-ordering rule above.
// used[i] = nums[i] is in cur. T: O(n * n!), S: O(n).
vector<vector<int>> permuteUnique(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    vector<vector<int>> out;
    vector<bool> used(nums.size(), false);
    vector<int> cur;
    dfs(nums, used, cur, out);
    return out;
}

void test(const string& label, vector<int> nums, vector<vector<int>> want) {
    vector<vector<int>> got = permuteUnique(nums);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("one-twin", {2, 1, 2}, {{1, 2, 2}, {2, 1, 2}, {2, 2, 1}});
    test("all-equal", {3, 3, 3}, {{3, 3, 3}});
    test("distinct", {1, 2}, {{1, 2}, {2, 1}});
}
