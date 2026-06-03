// post-order dfs, T: O(n), S: O(h) stack frames

#include <cstddef> // nullptr
#include <algorithm> // std::max
#include <climits> // INT_MIN

class Solution {
public:
    int maxPathSum(TreeNode* root) {
        int best = INT_MIN;
        dfs(root, best);
        return best;
    }

private:
    int dfs(TreeNode* node, int& best) {
        if (!node) { return 0; }

        int left = std::max(0, dfs(node->left, best));
        int right = std::max(0, dfs(node->right, best));

        best = std::max(best, node->val + left + right);
        return node->val + std::max(left, right);
    }
};

// std::max(0, dfs(...)): neg branch pruning, handles all neg trees
// best = INT_MIN: answer is the least neg val
// per dfs() call: update global best, return single-arm max
