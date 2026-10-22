// LC 39 Combination Sum: candidates are distinct positive ints, each reusable any number
// of times. Return every unique combination that sums to target.
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::sort;

void dfs(const vector<int>& cands, int start, int remain, vector<int>& cur,
         vector<vector<int>>& out) {
    if (remain == 0) {
        out.push_back(cur);
        return;
    }
    int n = cands.size();
    for (int i = start; i < n; i++) {
        // sorted, so every later candidate is too big as well
        if (cands[i] > remain) { break; }
        cur.push_back(cands[i]);
        // i, not i + 1: the same candidate may be reused
        dfs(cands, i, remain - cands[i], cur, out);
        cur.pop_back();
    }
}

// Backtracking over a sorted array; start index forbids going back, so each multiset is
// built once. cur is the partial combination. T: O(n^(target/min)), S: O(target/min).
vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> out;
    vector<int> cur;
    dfs(candidates, 0, target, cur, out);
    return out;
}

void test(const string& label, vector<int> cands, int target, vector<vector<int>> want) {
    vector<vector<int>> got = combinationSum(cands, target);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("reuse", {3, 2}, 6, {{2, 2, 2}, {3, 3}});
    test("none", {5, 7}, 3, {});
    test("single-exact", {4}, 4, {{4}});
}
