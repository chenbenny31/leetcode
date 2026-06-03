// recursive mid-point, T: O(n), S: O(logn) stack frame

#include <vector>
#include <cstddef> // nullptr

class Solution {
public:
    TreeNode* sortedArrayToBST(std::vector<int>& nums) {
        return build(nums, 0, static_cast<int>(nums.size()) - 1);
    }

private:
    TreeNode* build(std::vector<int>& nums, int lo, int hi) {
        if (lo > hi) { return nullptr; }
        int mid = lo + (hi - lo) / 2;
        TreeNode* node = new TreeNode(nums[mid]);
        node->left = build(nums, lo, mid - 1);
        node->right = build(nums, mid + 1, hi);
        return node;
    }
};

// O(logn) stack frame: split at midpoint, perfectly balanced tree
// new TreeNode per elem: in prod use pool alloc for contiguous node storage

