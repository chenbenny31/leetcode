// recursive, T: O(n), S: O(h) stack frames

#include <cstddef>

class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        if (!p && !q) { return true; }
        if (!p || !q) { return false; }
        if (p->val != q->val) { return false; }
        return isSameTree(p->left, q->left)
            && isSameTree(p->right, q->right);
    }
};

// iterative, T: O(n), S: O(h)

#include <vector>
#include <stack>
#include <cstddef> // nullptr
#include <utility> // std::pair

class Solution {
public:
    bool isSameTree(TreeNode* p, TreeNode* q) {
        std::vector<std::pair<TreeNode*, TreeNode*>> buf;
        buf.reserve(32);
        std::stack<std::pair<TreeNode*, TreeNode*>,
                   std::vector<std::pair<TreeNode*, TreeNode*>>> stk(std::move(buf));
        stk.push({p, q});

        while (!stk.empty()) {
            auto [a, b] = stk.top(); stk.pop();
            if (!a && !b) { continue; }
            if (!a || !b) { return false; }
            if (a->val != b->val) { return false; }
            stk.push({a->left, b->left});
            stk.push({a->right, b->right});
        }
        return true;
    }
};

// iter dfs not bfs: std::queue use std::deque intro segment and pointer chasing, std::stack use std::vector for reserve and cache-friendly
// reserve(32): reasonable tree depth estimate
