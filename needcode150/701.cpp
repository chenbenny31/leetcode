// recursive, T: O(h), S: O(h), stack frames

#include <cstddef> // nullptr

class Solution {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        if (!root) { return new TreeNode(val); }
        if (root->val > val) { root->left = insertIntoBST(root->left, val); }
        else { root->right = insertIntoBST(root->right, val); }
        return root;
    }
};

// iterative, T: O(h), S: O(1)

#include <cstddef>

class Solution {
public:
    TreeNode* insertIntoBST(TreeNode* root, int val) {
        TreeNode* node = new TreeNode(val);
        if (!root) { return node; }

        TreeNode* curr = root;
        TreeNode* parent = nullptr;
        while (curr) {
            parent = curr;
            curr = (curr->val > val) ? curr->left : curr->right;
        }
        if (parent->val > val) { parent->left = node; }
        else { parent->right = node; }
        return root;
    }
};

// insert as leaf: BST insertion never restructures existing nodes
