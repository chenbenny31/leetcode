// inorder traversal, T: O(n), S: O(h) stack frames

#include <cstddef> // nullptr
#include <climits> // LLONG_MIN

class Solution {
public:
    bool isValidBST(TreeNode* root) {
        long long prev = LLONG_MIN;
        return inorder(root, prev);
    }

private:
    bool inorder(TreeNode* node, long long& prev) {
        if (!node) { return true; }
        if (!inorder(node->left, prev)) { return false; }
        if (node->val <= prev) { return false; }
        prev = node->val;
        return inorder(node->right, prev);
    }
};

// range validation, S: O(n), S: O(h) stack frames

#include <cstddef> // nullptr
#include <climits> // LLONG_MIN, LLONG_MAX

class Solution {
public:
    bool isValidBST(TreeNode* root) {
        return validate(root, LLONG_MIN, LLONG_MAX);
    }

private:
    bool validate(TreeNode* node, long long lo, long long hi) {
        if (!node) { return true; }
        if (node->val <= lo || node->val >= hi) { return false; }
        return validate(node->left, lo, node->val)
            && validate(node->right, node->val, hi);
    }
};

// long long bounds mandatory: LLONG_MIN/LLONG_MAX safe for int range
// short-circuit && in range validation: prunes entire right subtree
// <= not <: BST requires strictly increasing order, equal values violates it
// inorder is preferred due to less frame size and more likely resident in L1
