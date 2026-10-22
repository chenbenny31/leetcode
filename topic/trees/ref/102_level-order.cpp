// LC 102 Binary Tree Level Order Traversal: return the node values of a binary tree
// level by level, left to right.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
using std::cout;
using std::string;
using std::vector;
using std::queue;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// BFS; freeze the queue size at the start of each level to know where it ends.
// T: O(n), S: O(w) for the widest level w (O(n) for the output).
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> levels;
    if (root == nullptr) { return levels; }
    queue<TreeNode*> q;   // nodes of the current level, then their children
    q.push(root);
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
        levels.push_back(level);
    }
    return levels;
}

void test(const string& label, TreeNode* root, const vector<vector<int>>& want) {
    cout << label << (levelOrder(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode d(4), b(2, &d), c(3), a(1, &b, &c);
    test("three-levels", &a, {{1}, {2, 3}, {4}});
    TreeNode single(7);
    test("single", &single, {{7}});
    test("empty", nullptr, {});
}
