// LC 987 Vertical Order Traversal: root at (row 0, col 0), left child (row+1, col-1),
// right child (row+1, col+1). Return columns left to right; within a column sort by
// row, then by value.

#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::tuple;
using std::sort;

struct TreeNode {
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode(int v) : val(v) {}
};

// collect (col, row, val) for every node
void dfs(TreeNode* cur, int row, int col, vector<tuple<int, int, int>>& cells) {
    if (cur == nullptr) { return; }
    cells.push_back({col, row, cur->val});
    dfs(cur->left, row + 1, col - 1, cells);
    dfs(cur->right, row + 1, col + 1, cells);
}

// Sorting (col, row, val) gives the exact required order; cut it into columns.
// T: O(n log n), S: O(n).
vector<vector<int>> verticalTraversal(TreeNode* root) {
    // (col, row, val) for every node
    vector<tuple<int, int, int>> cells;
    dfs(root, 0, 0, cells);
    sort(cells.begin(), cells.end());
    vector<vector<int>> result;
    int prevCol = 0;
    for (int i = 0; i < (int)cells.size(); i++) {
        auto [col, row, val] = cells[i];
        if (i == 0 || col != prevCol) { result.push_back({}); }
        result.back().push_back(val);
        prevCol = col;
    }
    return result;
}

void test(const string& label, TreeNode* root, const vector<vector<int>>& want) {
    cout << label << (verticalTraversal(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    // 1 has children 2, 3; 2 has right child 6; 3 has left child 5
    TreeNode a(1), b(2), c(3), d(6), e(5);
    a.left = &b;
    a.right = &c;
    b.right = &d;
    c.left = &e;
    test("same-cell-by-value", &a, {{2}, {1, 5, 6}, {3}});
    test("single", &e, {{5}});
}
