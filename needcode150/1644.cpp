// post-order dfs, T: O(n), S: O(h) stack frames

#include <cstddef> // nullptr

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        bool found_p = false, found_q = false;
        TreeNode* lca = dfs(root, p, q, found_p, found_q);
        return (found_p && found_q) ? lca : nullptr;
    }

private:
    TreeNode* dfs(TreeNode* node, TreeNode* p, TreeNode* q, bool& found_p, bool& found_q) {
        if (!node) { return nullptr; }

        TreeNode* left = dfs(node->left, p, q, found_p, found_q);
        TreeNode* right = dfs(node->right, p, q, found_p, found_q);

        if (node == p) { found_p = true; return node; }
        if (node == q) { found_q = true; return node; }
        if (left && right) { return node; }
        return left ? left : right;
    }
};

// full traversal: no early exit, must verify both existence
// found_p/found_q set after children: postorder 
// found_p && found_q final guard
// bool& by reference: flag shared across all frames
