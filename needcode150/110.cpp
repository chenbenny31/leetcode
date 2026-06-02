// post-order dfs with sentinel, T: O(n), S: O(h) stack frames

#include <cstddef> // nullptr
#include <algorithm> // std::max
#include <cstdlib> // std::abs

class Solution {
public:
    bool isBalanced(TreeNode* root) {
        return height(root) != -1;
    }

private:
    int height(TreeNode* node) {
        if (!node) { return 0; }

        int left = height(node->left);
        if (left == -1) { return -1; }

        int right = height(node->right);
        if (right == -1) { return -1; }

        if (std::abs(left - right) > 1) { return -1; }
        return 1 + std::max(left, right);
    }
};

// -1 sentinel: encode and short-circuits
// post-order mandatory: subtree check heights first
// std::abs(left-right) > 1: handles negative case
// return value of height: preserve info for curr node's computation
