// 25 · Binary Tree Level Order Traversal — LC 102, 199
// Mother problem: select/07-trees/01-level-order.cpp
//
// Google reports: LC 102 ×2, right side view ×2, alternating-color roots ×4 and tree
// roots (validate tree, 24), same color per depth
//
// LC 102: return node values level by level, top first, each left to right; null root →
// {}. Up to ~2000 nodes.
//
// Core idea
// - BFS; at the start of a level the queue holds exactly that level, so save
//   `levelSize = q.size()` before the inner loop and pop that many.
// - Push only non-null children. T O(n), S O(n).
//
// Follow-ups
// - right side view (LC 199) → same BFS, keep the node at i == levelSize - 1 (not "follow
//   right pointers": a deeper left branch shows)
// - zigzag (LC 103) → reverse every other finished level; never change push order
// - no queue → DFS with depth, open a level when depth == levels.size()
// - bottom-up (LC 107) → reverse levels at the end
// - per-level average / max (LC 637 / 515) → long long sum / max inside the level loop
// - min depth (LC 111) → return the level count at the first leaf popped
// - every depth one color → check each level against its first node; root chosen from
//   undirected edges → `uniformDepthColorRoot` in 26-validate-tree
// - max width with null gaps (LC 662) → queue of (node, index), children 2i+1 / 2i+2,
//   rebase on the level's first index to avoid overflow

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
