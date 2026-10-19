// Apple (hack2hire): all subsets of arr (each element used at most once) whose sum is
// exactly target, via backtracking.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// path = chosen elements so far, sum = their total
void dfs(const vector<int>& arr, int target, int start, int sum, vector<int>& path,
         vector<vector<int>>& result) {
    if (sum == target) { result.push_back(path); }
    int n = arr.size();
    for (int i = start; i < n; i++) {
        path.push_back(arr[i]);
        dfs(arr, target, i + 1, sum + arr[i], path, result);
        path.pop_back();
    }
}

// every subset is one node of the include/exclude tree: 2^n nodes, copying a hit is O(n).
// T: O(2^n * n), S: O(n) recursion + path, plus the output (up to 2^n subsets).
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
    test("two-hits", {1, 2, 3}, 3, {{1, 2}, {3}});
    test("none", {4, 6}, 3, {});
    test("zero", {1, 2}, 0, {{}});
}
