// LC 102 Binary Tree Level Order Traversal (Medium) — BFS by level size
// return node values level by level, top first, each level left -> right
//
// clarify:
// root null -> return {}
// dup values -> ok
// skewed tree -> depth = n; BFS has no recursion
// output shape -> one vector per level

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

// core idea: BFS; at level start the queue holds exactly that level -> pop `size` nodes
// T: O(n), S: O(n)
vector<vector<int>> levelOrder(TreeNode* root) {
    vector<vector<int>> levels;
    if (!root) { return levels; }
    // nodes of cur level, then their children
    queue<TreeNode*> q;
    q.push(root);
    while (!q.empty()) {
        // save first: queue grows while this level is popped
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
    TreeNode m4(4);
    TreeNode m2(2, &m4);
    TreeNode m3(3);
    TreeNode m1(1, &m2, &m3);
    test("left-deeper", &m1, {{1}, {2, 3}, {4}});
    TreeNode single(7);
    test("single", &single, {{7}});
    test("empty", nullptr, {});
    return 0;
}

// follow-up:
// zigzag (LC 103) -> same BFS, reverse every other finished level
// right side view (LC 199) -> keep last node of each level (i == levelSize - 1)
// no queue / recursive -> pre-order DFS w/ depth; new level when depth == levels.size()
// bottom-up (LC 107) -> levelOrder, then reverse the list of levels
// per-level avg / max (LC 637 / 515) -> long long sum or max inside the level loop
// min depth (LC 111) -> count levels, return at first leaf popped
// every depth one color (Google J30) -> per level check all nodes = first node's color
// max width incl. null gaps (LC 662) -> queue of (node, idx), kids 2i + 1, 2i + 2; rebase
