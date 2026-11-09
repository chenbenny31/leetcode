// LC 124 Binary Tree Maximum Path Sum (Hard). Pattern: post-order gain + global best.
// A path is any chain of adjacent nodes, each used once, not necessarily through the
// root. Return the max path sum; the tree is non-empty and values may be negative.

#include <algorithm>
#include <iostream>
#include <string>

using std::cout;
using std::max;
using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(x), left(l), right(r) {}
};

// returns the best downward chain from node; best = best path bending anywhere so far
int dfs(TreeNode* node, int& best) {
    if (!node) { return 0; }
    // a negative branch is better dropped
    int leftGain = max(dfs(node->left, best), 0);
    int rightGain = max(dfs(node->right, best), 0);
    best = max(best, node->val + leftGain + rightGain);
    // a parent can extend only one side
    return node->val + max(leftGain, rightGain);
}

// Every path bends at one node: try both sides at each node. T: O(n), S: O(h).
int maxPathSum(TreeNode* root) {
    int best = root->val;
    dfs(root, best);
    return best;
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (maxPathSum(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    // -3(4(6, 7), 5)
    TreeNode a6(6);
    TreeNode a7(7);
    TreeNode a4(4, &a6, &a7);
    TreeNode a5(5);
    TreeNode a3(-3, &a4, &a5);
    test("bend-below-root", &a3, 17);
    // 2(-1(4), 3)
    TreeNode b4(4);
    TreeNode b1(-1, &b4);
    TreeNode b3(3);
    TreeNode b2(2, &b1, &b3);
    test("through-negative", &b2, 8);
    TreeNode c1(-1);
    TreeNode c2(-2);
    TreeNode c3(-3, &c1, &c2);
    test("all-negative", &c3, -1);
}

// follow-up: diameter (LC 543) -> same skeleton on edge counts: best = max(best, l + r),
//   return 1 + max(l, r)
// follow-up: return the path -> record the best bend node, then follow larger gains down
