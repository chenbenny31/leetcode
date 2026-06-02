// recursive + hash-map, T: O(n), S: O(n)

#include <vector>
#include <unordered_map>
#include <cstddef> // nullptr

class Solution {
public:
    TreeNode* buildTree(std::vector<int>& preorder,
                        std::vector<int>& inorder) {
        int n = static_cast<int>(inorder.size());
        std::unordered_map<int, int> map; // inorder val: idx
        map.reserve(n);
        map.max_load_factor(0.25f);
        for (int i = 0; i < n; i++) { map[inorder[i]] = i; }

        int pre_idx = 0;
        return build(preorder, map, pre_idx, 0, n - 1);
    }

private:
    TreeNode* build(std::vector<int>& preorder,
                    std::unordered_map<int, int>& map,
                    int& pre_idx, int in_lo, int in_hi) {
        if (in_lo > in_hi) { return nullptr; }

        int val = preorder[pre_idx];
        pre_idx++;
        int in_mid = map[val];

        TreeNode* node = new TreeNode(val);
        node->left = build(preorder, map, pre_idx, in_lo, in_mid - 1);
        node->right = build(preorder, map, pre_idx, in_mid + 1, in_hi);
        return node;
    }
};

// pre_idx by ref: global adv, each call consumes one preorder elem
// left before right mandatory: left subtree must be built before right to align preorder
// cache hash-map lookup is random access, could use pool alloc for each node
