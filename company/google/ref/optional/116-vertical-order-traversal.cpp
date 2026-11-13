// 116 · Binary Tree Vertical Order Traversal — LC 314
// Mother: select/optional/07-trees/10-vertical-order-traversal.cpp
// return node values column by column, left to right; inside a column top to bottom
// same row and column -> left to right
//
// clarify:
// columns -> root = 0, left child col - 1, right child col + 1
// tie on same row and column -> left to right; by value = follow-up (LC 987)
// empty tree -> {}

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

// core idea: BFS carrying each node's col, append val to ordered map col -> list
// BFS order makes each column top to bottom and ties left to right
// T: O(n log n), S: O(n)
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
    TreeNode n4{4};
    TreeNode n6{6};
    TreeNode n5{5};
    TreeNode n7{7};
    TreeNode n2{2, &n4, &n6};
    TreeNode n3{3, &n5, &n7};
    TreeNode n1{1, &n2, &n3};
    test("tie-left-to-right", &n1, {{4}, {2}, {1, 6, 5}, {3}, {7}});
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

// follow-up:
// ties by smaller value (LC 987) -> DFS records (col, row, val), sort, group by col
