// ByteDance (1p3a 1191916, round 1 "力扣參漪巳") / LC 314: vertical order traversal:
// columns left to right (left child = col - 1, right child = col + 1); inside a column
// top to bottom, and left to right within the same row.
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

// BFS visits rows top to bottom, left to right: exactly the order needed inside a column.
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
        TreeNode* cur = q.front().first;
        int col = q.front().second;
        q.pop();
        columns[col].push_back(cur->val);
        if (cur->left != nullptr) {
            q.push({cur->left, col - 1});
        }
        if (cur->right != nullptr) {
            q.push({cur->right, col + 1});
        }
    }
    for (const auto& [col, values] : columns) {
        result.push_back(values);
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

// follow-up: same row + col sorted by value (LC 987) -> collect (col, row, val), sort;
//   O(n log n)
// follow-up: O(n) -> track minCol / maxCol, unordered_map<int, vector<int>>, emit
//   minCol..maxCol in order
// follow-up: DFS instead of BFS -> store row too, stable-sort each column by row;
//   preorder alone breaks top-to-bottom
// follow-up: top / bottom view -> first / last value seen per column in BFS
