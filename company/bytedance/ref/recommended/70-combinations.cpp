// ByteDance (hack2hire) / LC 77: return all combinations of k numbers chosen from 1..n.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

// extend path with numbers >= start until it holds k numbers
void backtrack(int n, int k, int start, vector<int>& path, vector<vector<int>>& result) {
    if ((int)path.size() == k) {
        result.push_back(path);
        return;
    }
    // prune: [i, n] must still hold the k - path.size() numbers we need
    for (int i = start; i <= n - (k - (int)path.size()) + 1; i++) {
        path.push_back(i);
        backtrack(n, k, i + 1, path, result);
        path.pop_back();
    }
}

// Backtracking with a start index. T: O(k * C(n, k)), S: O(k) besides the output.
vector<vector<int>> combine(int n, int k) {
    // every combination, in lexicographic order
    vector<vector<int>> result;
    // numbers picked so far, increasing
    vector<int> path;
    backtrack(n, k, 1, path, result);
    return result;
}

void test(const string& label, int n, int k, const vector<vector<int>>& want) {
    cout << label << (combine(n, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("lc-ex1", 4, 2, {{1, 2}, {1, 3}, {1, 4}, {2, 3}, {2, 4}, {3, 4}});
    test("k-==-1", 3, 1, {{1}, {2}, {3}});
    test("k-==-n", 3, 3, {{1, 2, 3}});
}

// follow-up: subsets (LC 78) -> record path at every call, not only at size k;
//   O(n * 2^n)
// follow-up: combination sum (LC 39, reuse) -> recurse with i, not i + 1; sort and
//   break once candidate > remaining
// follow-up: input with duplicates (LC 40 / 90) -> sort, skip i > start && a[i] == a[i-1]
