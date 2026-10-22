// LC 103 Binary Tree Zigzag Level Order Traversal: return the node values level by
// level, alternating direction: left to right, then right to left, and so on.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::reverse;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// Plain level-order BFS, then reverse every second level. T: O(n), S: O(n).
vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    vector<vector<int>> levels;
    if (root == nullptr) { return levels; }
    queue<TreeNode*> q;   // nodes of the current level, then their children
    q.push(root);
    bool leftToRight = true;
    while (!q.empty()) {
        int width = q.size();
        vector<int> level;
        for (int i = 0; i < width; i++) {
            TreeNode* cur = q.front();
            q.pop();
            level.push_back(cur->val);
            if (cur->left != nullptr) { q.push(cur->left); }
            if (cur->right != nullptr) { q.push(cur->right); }
        }
        if (!leftToRight) { reverse(level.begin(), level.end()); }
        levels.push_back(level);
        leftToRight = !leftToRight;
    }
    return levels;
}

void test(const string& label, TreeNode* root, const vector<vector<int>>& want) {
    cout << label << (zigzagLevelOrder(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n4(4), n5(5), n6(6), n7(7);
    TreeNode n2(2, &n4, &n5), n3(3, &n6, &n7), n1(1, &n2, &n3);
    test("three-levels", &n1, {{1}, {3, 2}, {4, 5, 6, 7}});
    TreeNode single(9);
    test("single", &single, {{9}});
    test("empty", nullptr, {});
}
