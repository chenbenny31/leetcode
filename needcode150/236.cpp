// post-order dfs, T: O(n), S: O(h) stack frames

#include <cstddef> // nullptr

class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root) { return nullptr; }
        if (root == p || root == q) { return root; }

        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);

        if (left && right) { return root; }
        return left ? left : right;
    }
};

// case: !root for null subtree; root==p || root==q for found target; left && right for p in left and q in right; non-null left/right, propagate up
