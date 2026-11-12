// LC 102 Binary Tree Level Order Traversal (Medium). Pattern: BFS by level size.
// Return the node values level by level, left to right.
//
// Clarify:
// - tree given as TreeNode* root (left / right pointers); root may be null -> return {}
// - values any int, duplicates fine (they are only copied)
// - up to ~2000 nodes (LC) -> O(n) BFS; BFS has no recursion, so a skewed tree is fine
//   (the DFS follow-up recurses as deep as the tree)
// - return one vector per level, top level first, each left to right
// Idea: BFS with a queue. When a level starts, the queue holds exactly that level, so
//   save q.size() and pop that many nodes, pushing their children for the next level.
//   Each node is pushed and popped once.
// Pitfalls:
// - save levelSize = q.size() before the inner loop; the queue grows while you pop
// - return before pushing a null root; push only non-null children
// - right side view = last node of each level (i == levelSize - 1), not "follow the
//   right pointers": a deeper left branch shows under a short right one
// - zigzag: reverse the finished level; never change the order children are pushed
// Tests to write: three levels with missing children; left branch deeper; right branch
//   deeper; single node; empty tree.

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

// pre-order, left before right: appends cur's value to levels[depth], opening the level
// the first time that depth is reached
void dfs(TreeNode* cur, int depth, vector<vector<int>>& levels) {
    if (!cur) { return; }
    if (depth == (int)levels.size()) { levels.push_back({}); }
    levels[depth].push_back(cur->val);
    dfs(cur->left, depth + 1, levels);
    dfs(cur->right, depth + 1, levels);
}

// follow-up "without a queue": DFS with the depth as a parameter. Left before right keeps
// each level in left-to-right order. T: O(n), S: O(h) recursion (O(n) on a skewed tree).
// Pitfall: open a level only when depth == levels.size(), never on every visit.
vector<vector<int>> levelOrderDfs(TreeNode* root) {
    vector<vector<int>> levels;
    dfs(root, 0, levels);
    return levels;
}

void test(const string& label, TreeNode* root, const vector<vector<int>>& want) {
    bool ok = levelOrder(root) == want && levelOrderDfs(root) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
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
    test("left-deeper", &m1, {{1}, {2, 3}, {4}});
    testRightSide("right-left-deeper", &m1, {1, 3, 4});
    testRightSide("right-empty", nullptr, {});
    // 1(2, 3(6, _)): the last level is opened from the right subtree
    TreeNode k6(6);
    TreeNode k2(2);
    TreeNode k3(3, &k6);
    TreeNode k1(1, &k2, &k3);
    test("right-deeper", &k1, {{1}, {2, 3}, {6}});
}

// follow-up: recursive / no queue -> levelOrderDfs above
// follow-up: bottom-up (LC 107) -> levelOrder, then
//     reverse(levels.begin(), levels.end());
// follow-up: per-level average / max (LC 637 / 515) -> inside the level loop keep
//     long long sum = 0;   // sum += cur->val; after the loop: (double)sum / levelSize
// follow-up: min depth (LC 111) -> count levels and return at the first leaf popped:
//     if (!cur->left && !cur->right) { return depth; }
// follow-up: every depth one color (Google J30) -> after each level check that all its
//   nodes have the first node's color; with the root to be chosen from undirected edges
//   -> uniformDepthColorRoot in company/google/ref/recommended/25-validate-tree
// follow-up: max width incl. null gaps (LC 662) -> queue of (node, index); children get
//   2 * i + 1 and 2 * i + 2; width = last index - first index + 1. Rebase on the level's
//   first index (saved as firstIdx when the level starts), or a deep chain overflows
//   even long long:
//     long long i = q.front().second - firstIdx;
