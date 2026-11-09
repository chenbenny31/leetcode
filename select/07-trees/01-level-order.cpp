// LC 102 Binary Tree Level Order Traversal (Medium). Pattern: BFS by level size.
// Return the node values level by level, left to right.

#include <iostream>
#include <queue>
#include <string>
#include <vector>

using std::cout;
using std::queue;
using std::string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(x), left(l), right(r) {}
};

// BFS; the queue size at the start of a level is that level's size. T: O(n), S: O(n).
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> levels;
    if (!root) { return levels; }
    // nodes of the current level, then their children
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int levelSize = q.size();
        vector<int> level;
        for (int i = 0; i < levelSize; i++) {
            TreeNode* cur = q.front();
            q.pop();
            level.push_back(cur->val);
            if (cur->left) { q.push(cur->left); }
            if (cur->right) { q.push(cur->right); }
        }
        levels.push_back(level);
    }
    return levels;
}

void test(const string& label, TreeNode* root, const vector<vector<int>>& want) {
    cout << label << (levelOrder(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    // 1(2(_, 4), 3(_, 5))
    TreeNode n4(4);
    TreeNode n5(5);
    TreeNode n2(2, nullptr, &n4);
    TreeNode n3(3, nullptr, &n5);
    TreeNode n1(1, &n2, &n3);
    test("three-levels", &n1, {{1}, {2, 3}, {4, 5}});
    TreeNode single(7);
    test("single", &single, {{7}});
    test("empty", nullptr, {});
}

// follow-up: zigzag (LC 103) -> same BFS, reverse every other level before saving it
// follow-up: right side view (LC 199) -> keep only the last node of each level
