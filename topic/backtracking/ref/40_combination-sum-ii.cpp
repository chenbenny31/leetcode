// LC 40 Combination Sum II: candidates may repeat, each used at most once. Return every
// unique combination that sums to target.
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
        if (cands[i] > remain) { break; }
        // equal to the previous sibling: that branch already covered this value here
        if (i > start && cands[i] == cands[i - 1]) { continue; }
        cur.push_back(cands[i]);
        dfs(cands, i + 1, remain - cands[i], cur, out);
        cur.pop_back();
    }
}

// Backtracking over a sorted array; skipping equal siblings removes duplicate
// combinations. cur is the partial combination. T: O(2^n), S: O(n).
vector<vector<int>> combinationSum2(vector<int>& candidates, int target) {
    sort(candidates.begin(), candidates.end());
    vector<vector<int>> out;
    vector<int> cur;
    dfs(candidates, 0, target, cur, out);
    return out;
}

void test(const string& label, vector<int> cands, int target, vector<vector<int>> want) {
    vector<vector<int>> got = combinationSum2(cands, target);
    sort(got.begin(), got.end());
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("duplicates", {2, 1, 2, 1}, 3, {{1, 2}});
    test("repeat-allowed-once-each", {1, 1, 1}, 2, {{1, 1}});
    test("none", {4, 5}, 3, {});
}
