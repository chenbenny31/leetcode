// 111 · Binary Tree Vertical Order Traversal — LC 314
// Mother problem: select/optional/07-trees/10-vertical-order-traversal.cpp
//
// Clarify
// - coordinates? → root (row 0, col 0); left (row + 1, col − 1), right (row + 1, col + 1)
// - same row and column? → left to right (BFS order)
// - output? → columns left to right, each top to bottom
// - empty tree? → {}
//
// Core idea
// - BFS carrying each node's column; append values to map col → list in visit order.
// - BFS (not DFS) is what makes each column top to bottom and ties left to right.
//
// Tests: tie left to right, top to bottom, single, empty
//
// Follow-ups
// - LC 987 (ties by smaller value) → DFS records (col, row, val), sort the tuples, group
//   consecutive tuples with the same column; O(n log n)
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::map;
using std::queue;
using std::pair;

struct TreeNode {
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

// BFS with columns; the ordered map gives columns left to right.
// T: O(n log n), S: O(n).
vector<vector<int>> verticalOrder(TreeNode* root) {
    if (!root) { return {}; }
    // columns[col] = values in that column, in BFS order
    map<int, vector<int>> columns;
    // (node, its column)
    queue<pair<TreeNode*, int>> q;
    q.push({root, 0});
    while (!q.empty()) {
        auto [cur, col] = q.front();
        q.pop();
        columns[col].push_back(cur->val);
        if (cur->left) { q.push({cur->left, col - 1}); }
        if (cur->right) { q.push({cur->right, col + 1}); }
    }
    vector<vector<int>> result;
    for (auto& entry : columns) { result.push_back(entry.second); }
    return result;
}

void test(const string& label, TreeNode* root, const vector<vector<int>>& want) {
    cout << label << (verticalOrder(root) == want ? " PASS\n" : " FAIL\n");
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
    test("tie-left-to-right", &n1, {{4}, {2}, {1, 6, 5}, {3}, {7}});
    // 1(2(_, 4(_, 5)), 3): DFS would put 5 (row 3) before 3 (row 1) in column 1
    TreeNode m5{5};
    TreeNode m4{4, nullptr, &m5};
    TreeNode m2{2, nullptr, &m4};
    TreeNode m3{3};
    TreeNode m1{1, &m2, &m3};
    test("top-to-bottom", &m1, {{2}, {1, 4}, {3, 5}});
    TreeNode solo{9};
    test("single", &solo, {{9}});
    test("empty", nullptr, {});
}
