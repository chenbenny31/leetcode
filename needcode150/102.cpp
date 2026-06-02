// bfs, T: O(n), S: O(w)

#include <vector>
#include <queue>
#include <cstddef> // nullptr

class Solution {
public:
    std::vector<std::vector<int>> levelOrder(TreeNode* root) {
        std::vector<std::vector<int>> out;
        if (!root) { return out; }

        std::queue<TreeNode*> q;
        q.push(root);

        std::vector<int> level;

        while (!q.empty()) {
            int sz = static_cast<int>(q.size());
            level.clear();
            level.reserve(sz);

            for (int i = 0; i < sz; i++) {
                TreeNode* node = q.front(); q.pop();
                level.push_back(node->val);
                if (node->left) { q.push(node->left); }
                if (node->right) { q.push(node->right); }
            }
            out.push_back(std::move(level));
        }
        return out;
    }
};

// dfs, T: O(n), S: O(h)

#include <vector>
#include <cstddef>

class Solution {
public:
    std::vector<std::vector<int>> levelOrder(TreeNode* root) {
        std::vector<std::vector<int>> out;
        dfs(root, 0, out);
        return out;
    }

private:
    void dfs(TreeNode* node, int depth, std::vector<std::vector<int>>& out) {
        if (!node) { return; }
        if (depth == static_cast<int>(out.size())) { out.push_back({}); }
        out[depth].push_back(node->val);
        dfs(node->left, depth + 1, out);
        dfs(node->right, depth + 1, out);
    }
};

// level pre-alloc outside loop
// depth == out.size(): first visit at depth
// std::queue no reserve: in prod replace by circular buf or two-vec
