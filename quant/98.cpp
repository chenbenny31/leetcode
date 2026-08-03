// recur

#include <climits> // LLONG_MIN, LLONG_MAX

class Solution {
private:
    bool validate(TreeNode* node, long long lo, long long hi) {
        if (!node) { return true; }
        if (node->val <= lo || node->val >= hi) { return false; }
        return validate(node->left, lo, node->val)
            && validate(node->right, node->val, hi);
    }
public:
    bool isValidBST(TreeNode* root) {
        return validate(root, LLONG_MIN, LLONG_MAX);
    }
};

// iter

#include <stack>
#include <vector>

class Solution {
public:
    bool isValidBST(TreeNode* root) {
        std::vector<TreeNode*> buf;
        std::stack<TreeNode*, std::vector<TreeNode*>> stk(std::move(buf));

        TreeNode* curr = root;
        TreeNode* prev = nullptr; // inorder-predecessor

        while (curr || !stk.empty()) {
            while (curr) {
                stk.push(curr);
                curr = curr->left;
            }
            curr = stk.top(); stk.pop();

            if (prev && prev->val >= curr->val) { return false; }

            prev = curr;
            curr = curr->right;
        }
        return true;
    }
};
