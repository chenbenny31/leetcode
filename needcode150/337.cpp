// postorder dfs with pair, T: O(n), S: O(h) stack frames

#include <cstddef> // nullptr
#include <algorithm> // std::max
#include <utility> // std::pair

class Solution {
public:
    int rob(TreeNode* root) {
        auto [rob_val, skip_val] = dfs(root);
        return std::max(rob_val, skip_val);
    }

private:
    std::pair<int, int> dfs(TreeNode* node) {
        if (!node) { return {0, 0}; }

        auto [l_rob, l_skip] = dfs(node->left);
        auto [r_rob, r_skip] = dfs(node->right);

        int rob_curr = node->val + l_skip + r_skip;
        int skip_curr = std::max(l_rob, l_skip) + std::max(r_rob, r_skip);

        return {rob_curr, skip_curr};
    }
};

// {rob, skip} pair encodes two state, post-order mandatory
// auto [l_rob, l_skip] = dfs(node->left): destructuring of std::pair
