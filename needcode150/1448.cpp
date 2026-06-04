// dfs with running max, T: O(n), S: O(h) stack frames

#include <cstddef> // nullptr
#include <algorithm> // std::max
#include <climits> // INT_MIN

class Solution {
public:
    int goodNodes(TreeNode* root) {
        return dfs(root, INT_MIN);
    }

private:
    int dfs(TreeNode* node, int cur_max) {
        if (!node) { return 0; }
        int is_good = (node->val >= cur_max) ? 1 : 0;
        cur_max = std::max(cur_max, node->val);
        return is_good + dfs(node->left, cur_max)
                    + dfs(node->right, cur_max);
    }
};

// iterative dfs, T: O(n), S: O(1)

#include <vector>
#include <stack>
#include <utility> // std::pair
#include <algorithm> // std::max
#include <climits> // INT_MIN
#include <cstddef> // nullptr

class Solution {
public:
    int goodNodes(TreeNode* root) {
        if (!root) { return 0; }

        std::vector<std::pair<TreeNode*, int>> buf;
        buf.reserve(32);
        std::stack<std::pair<TreeNode*, int>,
                   std::vector<std::pair<TreeNode*, int>>> stk(std::move(buf)); 
        stk.push({root, INT_MIN});
        int count = 0;

        while (!stk.empty()) {
            auto [node, cur_max] = stk.top(); stk.pop();
            if (node->val >= cur_max) { count++; }
            int next_max = std::max(cur_max, node->val);
            if (node->left) { stk.push({node->left, next_max}); }
            if (node->right) { stk.push({node->right, next_max}); }
        }
        return count;
    }
};

// INT_MIN seed root: root->val >= INT_MIN always true
// cur_max passed by value: each recursive call has its own copy
