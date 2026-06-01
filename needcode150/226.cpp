// recursive, T: O(n), S: O(h) stack frames, h = tree height

#include <cstddef> // nullptr
#include <algorithm> // std::swap

class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) { return nullptr; }
        std::swap(root->left, root->right);
        invertTree(root->left);
        invertTree(root->right);
        return root;
    }
};

// iterative bfs, T: O(n), S: O(w), w: max width

#include <queue>
#include <cstddef> // nullptr
#include <algorithm> // std::swap

class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) { return nullptr; }
        std::queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();
            std::swap(node->left, node->right);
            if (node->left) { q.push(node->left); }
            if (node->right) { q.push(node->right); }
        }
        return root;
    }
};

// std::swap on pointers: two register moves, no alloc
// recursive vs bfs-space: O(h) stack, O(logn) balanced, O(n) skwed vs O(w) queue, O(n) complete tree
