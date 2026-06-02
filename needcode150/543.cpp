// dfs with global max, T: O(n), S: O(h) stack frames

#include <algorithm> // std::max

class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int best = 0;
        dfs(root, best);
        return best;
    }

private:
    int dfs(TreeNode* node, int& best) {
        if (!node) { return 0; }
        int left = dfs(node->left, best);
        int right = dfs(node->right, best);
        best = std::max(best, left + right);
        return 1 + std::max(left, right);
    }
};

// best by ref: global update
// left + right at each node: diameter = left edge + right edge
// return height: can used to compute diameter for parent node
