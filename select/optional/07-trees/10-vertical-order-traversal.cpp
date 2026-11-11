// LC 987 Vertical Order Traversal (Hard). Pattern: collect (col, row, val), sort, group.
// Root at (row 0, col 0); left child (row + 1, col - 1), right child (row + 1, col + 1).
// Return columns left to right, each top to bottom; same row and column -> smaller first.

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
};

// record (col, row, val) for every node
void dfs(TreeNode* node, int row, int col, vector<tuple<int, int, int>>& cells) {
    if (!node) { return; }
    cells.push_back({col, row, node->val});
    dfs(node->left, row + 1, col - 1, cells);
    dfs(node->right, row + 1, col + 1, cells);
}

// Sorting the tuples gives exactly the required order: column, then row, then value.
// T: O(n log n), S: O(n).
vector<vector<int>> verticalTraversal(TreeNode* root) {
    // (col, row, val) of every node
    vector<tuple<int, int, int>> cells;
    dfs(root, 0, 0, cells);
    sort(cells.begin(), cells.end());

    vector<vector<int>> result;
    int prevCol = 0;
    for (int i = 0; i < (int)cells.size(); i++) {
        auto [col, row, val] = cells[i];
        // a new column starts a new group
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
    // 1(2(4, 6), 3(5, 7)): 6 and 5 share row 2, column 0
    TreeNode n4{4};
    TreeNode n6{6};
    TreeNode n5{5};
    TreeNode n7{7};
    TreeNode n2{2, &n4, &n6};
    TreeNode n3{3, &n5, &n7};
    TreeNode n1{1, &n2, &n3};
    test("tie-by-value", &n1, {{4}, {2}, {1, 5, 6}, {3}, {7}});
    TreeNode solo{9};
    test("single", &solo, {{9}});
    test("empty", nullptr, {});
}

// follow-up: LC 314 (same row -> left to right, no value sort) -> BFS with column, append
//   in visit order to map<int, vector<int>>
// follow-up: avoid the full sort -> map<col, vector<(row, val)>>, sort each column only
