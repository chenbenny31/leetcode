// return all subsets of array with sum == target

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

void dfs(const vector<int>& arr, int target, int start, int sum,
         vector<int>& path, vector<vector<int>>& result) {
    if (sum == target) { // value not specify un-negative
        result.push_back(path);
    }
    int n = arr.size();
    for (int i = start; i < n; i++) {
        path.push_back(arr[i]);
        dfs(arr, target, i + 1, sum + arr[i], path, result);
        path.pop_back();
    }
}

// T: O(2^n*n), S: O(n) recursion + path
vector<vector<int>> subsetsWithSum(const vector<int>& arr, int target) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(arr, target, 0, 0, path, result);
    return result;
}

void test(const string& label, const vector<int>& arr, int target,
          const vector<vector<int>>& want) {
    cout << label << (subsetsWithSum(arr, target) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("example", {2, 3, 5, 6, 8, 10}, 10, {{2, 3, 5}, {2, 8}, {10}});
    test("none", {4, 6}, 3, {});
    test("target 0", {1, 2}, 0, {{}});
}
