// bfs with direction flag, T: O(n), S: O(w)

#include <vector>
#include <queue>
#include <cstddef> // nullptr

class Solution {
public:
    std::vector<std::vector<int>> zigzagLevelOrder(TreeNode* root) {
        std::vector<std::vector<int>> out;
        if (!root) { return out; }

        std::queue<TreeNode*> q;
        q.push(root);
        bool left_to_right = true;

        std::vector<int> level;

        while (!q.empty()) {
            int sz = static_cast<int>(q.size());
            level.assign(sz, 0);

            for (int i = 0; i < sz; i++) {
                TreeNode* node = q.front(); q.pop();
                int idx = left_to_right ? i : sz - 1 - i;
                level[idx] = node->val;
                if (node->left) { q.push(node->left); }
                if (node->right) { q.push(node->right); }
            }
            out.push_back(std::move(level));
            left_to_right = !left_to_right;
        }
        return out;
    }
};

// level.assign(size, 0): assign resize and fills in one call
// bfs not dfs: dfs has insert(), O(w) per node, O(w2) total
