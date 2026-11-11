// LC 102 Binary Tree Level Order Traversal (Medium). Pattern: BFS by level size.
// Return the node values level by level, left to right.

#include <algorithm>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using std::cout;
using std::queue;
using std::reverse;
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

// LC 103: same BFS, reverse every other level before saving it. T: O(n), S: O(n).
vector<vector<int>> zigzagLevelOrder(TreeNode* root) {
    vector<vector<int>> levels;
    if (!root) { return levels; }
    // nodes of the current level, then their children
    queue<TreeNode*> q;
    q.push(root);
    bool rightToLeft = false;
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
        if (rightToLeft) { reverse(level.begin(), level.end()); }
        levels.push_back(level);
        rightToLeft = !rightToLeft;
    }
    return levels;
}

// LC 199: same BFS, keep only the last node of each level. T: O(n), S: O(n).
vector<int> rightSideView(TreeNode* root) {
    vector<int> view;
    if (!root) { return view; }
    // nodes of the current level, then their children
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; i++) {
            TreeNode* cur = q.front();
            q.pop();
            if (i == levelSize - 1) { view.push_back(cur->val); }
            if (cur->left) { q.push(cur->left); }
            if (cur->right) { q.push(cur->right); }
        }
    }
    return view;
}

void test(const string& label, TreeNode* root, const vector<vector<int>>& want) {
    cout << label << (levelOrder(root) == want ? " PASS\n" : " FAIL\n");
}

void testZigzag(const string& label, TreeNode* root, const vector<vector<int>>& want) {
    bool ok = zigzagLevelOrder(root) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testRightSide(const string& label, TreeNode* root, const vector<int>& want) {
    bool ok = rightSideView(root) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
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
    testZigzag("zigzag-three-levels", &n1, {{1}, {3, 2}, {4, 5}});
    testZigzag("zigzag-single", &single, {{7}});
    testZigzag("zigzag-empty", nullptr, {});
    testRightSide("right-all-right", &n1, {1, 3, 5});
    // 1(2(4, _), 3): 4 is seen from the right below the short right branch
    TreeNode m4(4);
    TreeNode m2(2, &m4);
    TreeNode m3(3);
    TreeNode m1(1, &m2, &m3);
    testRightSide("right-left-deeper", &m1, {1, 3, 4});
    testRightSide("right-empty", nullptr, {});
}
