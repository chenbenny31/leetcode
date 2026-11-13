// 28 · Binary Tree Level Order Traversal — LC 102, 199
// Mother: select/07-trees/01-level-order.cpp
// LC 102: node values level by level, top first, left to right; null root -> {}
// reported: LC 102 x2, right side view x2, color-root variants (29)
//
// clarify:
// null root -> {}
// node count (~2000) -> BFS queue is fine
// one list per level or a flat list? -> one list per level

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

// core idea: BFS; at level start the queue = exactly that level, so pop q.size() nodes
// T: O(n), S: O(n)
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

// follow-up:
// right side view (LC 199) -> keep node at i == levelSize - 1 (deeper left branch shows)
// zigzag (LC 103) -> reverse every other finished level; never change the push order
// no queue -> DFS w/ depth, open a new level when depth == levels.size()
// per-level average / max (LC 637 / 515) -> long long sum / max inside the level loop
// min depth (LC 111) -> return level count at the first leaf popped
// every depth one color, root from undirected edges -> uniformDepthColorRoot in 26
// max width w/ null gaps (LC 662) -> queue of (node, idx), children 2i + 1, 2i + 2
// LC 662 overflow -> rebase on the level's first idx
