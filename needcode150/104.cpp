// recursive dfs, T: O(n), S: O(h) stack frames

#include <cstddef> // nullptr
#include <algorithm> // std::max

class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) { return 0; }
        return 1 + std::max(maxDepth(root->left), maxDepth(root->right));
    }
};

// iterative dfs, T: O(n), S: O(h)

#include <stack>
#include <vector>
#include <cstddef> // nullptr
#include <utility> // std::pair
#include <algorithm> // std::max

class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) { return 0; }

        std::vector<std::pair<TreeNode*, int>> buf;
        buf.reserve(32);
        std::stack<std::pair<TreeNode*, int>,
                   std::vector<std::pair<TreeNode*, int>>> stk(std::move(buf));

        stk.push({root, 1});
        int best = 0;

        while (!stk.empty()) {
            auto [node, depth] = stk.top(); stk.pop();
            best = std::max(best, depth);
            if (node->left) { stk.push({node->left, depth + 1}); }
            if (node->right) { stk.push({node->right, depth + 1}); }
        }
        return best;
    }
};

// iterative bfs, T: O(n), S: O(w)

#include <queue>
#include <cstddef> // nullptr

class Solution {
public:
    int maxDepth(TreeNode* root) {
        if (!root) { return 0; }
        std::queue<TreeNode*> q;
        q.push(root);
        int depth = 0;

        while (!q.empty()) {
            int sz = static_cast<int>(q.size());
            depth++;
            for (int i = 0; i < sz; ++i) {
                TreeNode* node = q.front(); q.pop();
                if (node->left) { q.push(node->left); }
                if (node->right) { q.push(node->right); }
            }
        }
        return depth;
    }
};

// recursive: post-order, subtree computed before parent, tail-call opt on balanced tree
// bfs depth = level count: sz snapshot before inner level loop
