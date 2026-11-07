// LC 314 Binary Tree Vertical Order Traversal: list node values column by column, left to
// right; top to bottom within a column, left to right on the same row.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <utility>

using std::cout;
using std::map;
using std::pair;
using std::queue;
using std::string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// BFS keeps top-to-bottom and left-to-right order; the column is root = 0, left child
// col - 1, right child col + 1. T: O(n log n), S: O(n).
vector<vector<int>> verticalOrder(TreeNode* root) {
    vector<vector<int>> result;
    if (!root) { return result; }
    map<int, vector<int>> cols;                // column -> values in BFS order
    queue<pair<TreeNode*, int>> q;             // (node, its column)
    q.push({root, 0});

    while (!q.empty()) {
        auto [node, col] = q.front();
        q.pop();
        cols[col].push_back(node->val);
        if (node->left) { q.push({node->left, col - 1}); }
        if (node->right) { q.push({node->right, col + 1}); }
    }

    for (const auto& entry : cols) {
        result.push_back(entry.second);
    }

    return result;
}

void test(const string& label, TreeNode* root, const vector<vector<int>>& want) {
    cout << label << (verticalOrder(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n1(1), n2(2), n3(3), n5(5), n6(6);
    n1.left = &n2;
    n1.right = &n3;
    n2.right = &n5;
    n3.left = &n6;

    test("shared-column", &n1, {{2}, {1, 5, 6}, {3}});

    TreeNode only(9);
    test("single", &only, {{9}});
    test("empty", nullptr, {});
}
