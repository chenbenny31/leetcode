// prefix + hash-map, T: O(n), S: O(n)

#include <unordered_map>
#include <cstddef> // nullptr

class Solution {
public:
    int pathSum(TreeNode* root, int targetSum) {
        std::unordered_map<long long, int> freq;
        freq.reserve(1024);
        freq.max_load_factor(0.25f);
        freq[0] = 1; // empty path prefix
        return dfs(root, 0, targetSum, freq);
    }

private:
    int dfs(TreeNode* node, long long curr, int targetSum,
            std::unordered_map<long long, int>& freq) {
        if (!node) { return 0; }

        curr += node->val;
        int count = 0;
        auto it = freq.find(curr - targetSum);
        if (it != freq.end()) { count = it->second; }

        freq[curr]++;
        count += dfs(node->left, curr, targetSum, freq);
        count += dfs(node->right, curr, targetSum, freq);
        freq[curr]--; // backtrack

        return count;
    }
};

// freq[0] = 1 sentinel: handles paths starting from root
// freq[curr]-- backtrack: remove current prefix sum before returning to parent
// long long curr: node values up to 10^9, path length up to 10^3, prefix sum up to 10^12
// freq.find not freq[]: freq[] will inserts 0 into map, corrupting furture lookup
// backtrack not erase: erase will remove all occurrences
