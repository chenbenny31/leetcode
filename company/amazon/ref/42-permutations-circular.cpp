// List every order in which students 1..n can stand in a line.
// Follow-up: list every distinct arrangement around a circle, where rotations are the
// same arrangement and mirror images are different; there are (n - 1)! of them.

#include <iostream>
#include <vector>
#include <string>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::swap;

// fix cur[i] by swapping each remaining student into position i
void dfs(vector<int>& cur, int i, vector<vector<int>>& result) {
    int n = cur.size();
    if (i == n) {
        result.push_back(cur);
        return;
    }
    for (int j = i; j < n; j++) {
        swap(cur[i], cur[j]);
        dfs(cur, i + 1, result);
        swap(cur[i], cur[j]);
    }
}

// backtracking by swaps. T: O(n * n!), S: O(n) besides output.
vector<vector<int>> linear(int n) {
    vector<int> cur;
    for (int s = 1; s <= n; s++) { cur.push_back(s); }
    vector<vector<int>> result;
    dfs(cur, 0, result);
    return result;
}

// circle: rotating so student 1 is first picks one representative per class, so fix
// student 1 and permute the rest. T: O(n * (n-1)!).
vector<vector<int>> circular(int n) {
    vector<int> cur;
    for (int s = 1; s <= n; s++) { cur.push_back(s); }
    vector<vector<int>> result;
    dfs(cur, 1, result);
    return result;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<vector<int>> three = linear(3);
    check(three.size() == 6 && three[0] == vector<int>{1, 2, 3}, "line-three");
    check(linear(1) == vector<vector<int>>{{1}}, "line-one");
    vector<vector<int>> ring = {{1, 2, 3}, {1, 3, 2}};
    check(circular(3) == ring, "circle-three");
    check(circular(4).size() == 6, "circle-four");
}
