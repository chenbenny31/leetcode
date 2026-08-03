// bfs, level order

#include <vector>
#include <utility>

class Solution {
public:
    std::vector<std::vector<int>> levelOrder(TreeNode* root) {
        std::vector<std::vector<int>> out;
        if (!root) { return out; }

        std::vector<TreeNode*> q;
        q.push_back(root);
        int head = 0;

        std::vector<int> level;
        while (head < static_cast<int>(q.size())) {
            int levelEnd = static_cast<int>(q.size());
            level.clear();
            level.reserve(levelEnd - head);

            for (int i = head; i < levelEnd; i++) {
                TreeNode* node = q[i];
                level.push_back(node->val);
                if (node->left) { q.push_back(node->left); }
                if (node->right) { q.push_back(node->right); }
            }
            out.push_back(std::move(level));
            head = levelEnd;
        }
        return out;
    }
};

// dfs, carry depth

class Solution {
private:
    void dfs(TreeNode* node, int depth, std::vector<std::vector<int>>& out) {
        if (!node) { return; }
        if (depth == static_cast<int>(out.size())) { out.push_back({}); } // first node at this depth

        out[depth].push_back(node->val);
        dfs(node->left, depth + 1, out);
        dfs(node->right, depth + 1, out);
    }

public:
    std::vector<std::vector<int>> levelOrder(TreeNode* root) {
        std::vector<std::vector<int>> out;
        dfs(root, 0, out);
        return out;
    }
};
