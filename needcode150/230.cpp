// inorder dfs, T: O(h + k), S: O(h) stack frames

#include <cstddef> // nullptr

class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        int result = -1;
        inorder(root, k, result);
        return result;
    }

private:
    void inorder(TreeNode* node, int& k, int& result) {
        if (!node || result != -1) { return; }
        inorder(node->left, k, result);
        k--;
        if (k == 0) { result = node->val; return; }
        inorder(node->right, k, result);
    }
};

// iterative inorder, T: O(h + h), S: O(h) stack frames

#include <stack>
#include <vector>
#include <cstddef> // nullptr

class Solution {
public:
    int kthSmallest(TreeNode* root, int k) {
        std::vector<TreeNode*> buf;
        buf.reserve(32);
        std::stack<TreeNode*, std::vector<TreeNode*>> stk(std::move(buf));

        TreeNode* curr = root;
        while (curr || !stk.empty()) {
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }
            curr = stk.top(); stk.pop();
            k--;
            if (k == 0) { return curr->val; }
            curr = curr->right;
        }
        return -1;
    }
};

// result != -1 for early exit: guard both left and right subtree calls
// O(h + k) not O(n): traverse down to leftmost O(h) then k steps inorder O(k)
// buf.reserve(32): tree height estimate
// cache iter wins on explicit stk of ptr, contigous vec L1 warm
