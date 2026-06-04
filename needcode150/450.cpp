// recursive. T: O(h), S: O(h) stack frames

#include <cstddef> // nullptr

class Solution {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        if (!root) { return nullptr; }

        if (key < root->val) {
            root->left = deleteNode(root->left, key);
        } else if (key > root->val) {
            root->right = deleteNode(root->right, key);
        } else {
            if (!root->left) { return root->right; }
            if (!root->right) { return root->left; }

            TreeNode* succ = root->right; 
            while (succ->left) { succ = succ->left; } 
            root->val = succ->val; // replace with inorder successor
            root->right = deleteNode(root->right, succ->val);
        }
        return root;
    }
};

// iterative, T: O(h), S: O(1) 

#include <cstddef> // nullptr

class Solution {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        TreeNode dummy(0);
        dummy.left = root;
        TreeNode* parent = &dummy;
        TreeNode* curr = root;
        bool is_left = true;

        while (curr && curr->val != key) {
            parent = curr;
            if (key < curr->val) { curr = curr->left; is_left = true; }
            else { curr = curr->right; is_left = false; }
        }
        if (!curr) { return dummy.left; } // not found

        TreeNode* replace = nullptr;
        if (!curr->left) {
            replace = curr->right;
        } else if (!curr->right) {
            replace = curr->left;
        } else {
            TreeNode* succ_parent = curr;
            TreeNode* succ = curr->right;
            while (succ->left) {
                succ_parent = succ;
                succ = succ->left;
            }
            if (succ_parent == curr) { succ_parent->right = succ->right; }
            else { succ_parent->left = succ->right; }
            succ->left = curr->left;
            succ->right = curr->right;
            replace = succ;
        }

        if (is_left) { parent->left = replace; }
        else { parent->right = replace; }

        delete curr;
        return dummy.left;
    }
};

// three deletion cases: no child -> return nullptr; one child -> return existing child; two children -> find inorder succ
// dummy head: handles deletion of root
// is_left flat: tracks curr is left or right child of parent
// succ_parent = curr guard: when succ is direct child of curr
