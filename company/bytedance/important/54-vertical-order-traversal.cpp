// ByteDance (1p3a 1191916, round 1 "力扣參漪巳") / LC 314: return the vertical order
// traversal of a binary tree: columns from left to right (root at column 0, a left
// child is column - 1, a right child column + 1). Inside a column list nodes from top
// to bottom; nodes in the same row and column go left to right.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <utility>
#include <climits>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::map;
using std::pair;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// BFS visits nodes row by row and left to right inside a row, which is exactly the
// order required inside each column, so appending to the column's list is enough.
// T: O(n log n) for the column map, S: O(n).
vector<vector<int>> verticalOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (root == nullptr) {
        return result;
    }
    // columns[c] = node values in column c, in BFS order
    map<int, vector<int>> columns;
    // q = (node, its column)
    queue<pair<TreeNode*, int>> q;
    q.push({root, 0});
    while (!q.empty()) {
        auto [cur, col] = q.front();
        q.pop();
        columns[col].push_back(cur->val);
        if (cur->left != nullptr) {
            q.push({cur->left, col - 1});
        }
        if (cur->right != nullptr) {
            q.push({cur->right, col + 1});
        }
    }
    for (auto& [col, vals] : columns) {
        result.push_back(vals);
    }
    return result;
}

const int NIL = INT_MIN;

// Builds a tree from level order (NIL = no node) inside `nodes`, returns the root.
TreeNode* build(const vector<int>& level, vector<TreeNode>& nodes) {
    if (level.empty() || level[0] == NIL) {
        return nullptr;
    }
    nodes.reserve(level.size());
    nodes.push_back({level[0], nullptr, nullptr});
    queue<TreeNode*> q;
    q.push(&nodes.back());
    int i = 1;
    while (!q.empty() && i < (int)level.size()) {
        TreeNode* cur = q.front();
        q.pop();
        if (level[i] != NIL) {
            nodes.push_back({level[i], nullptr, nullptr});
            cur->left = &nodes.back();
            q.push(cur->left);
        }
        i++;
        if (i < (int)level.size() && level[i] != NIL) {
            nodes.push_back({level[i], nullptr, nullptr});
            cur->right = &nodes.back();
            q.push(cur->right);
        }
        i++;
    }
    return &nodes[0];
}

void test(const string& label, const vector<int>& level,
          const vector<vector<int>>& want) {
    vector<TreeNode> nodes;
    TreeNode* root = build(level, nodes);
    cout << label << (verticalOrder(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("empty", {}, {});
    test("full", {1, 2, 3, 4, 5, 6, 7}, {{4}, {2}, {1, 5, 6}, {3}, {7}});
    test("left-chain", {1, 2, NIL, 3}, {{3}, {2}, {1}});
    test("crossing", {1, 2, 3, NIL, 4, 5}, {{2}, {1, 4, 5}, {3}});
}
