// dfs + sorting, T: O(nlogn), S: O(n)

#include <vector>
#include <algorithm> // std::sort
#include <map> // std::map
#include <tuple>
#include <cstddef> // nullptr
#include <climits> // INT_MIN

class Solution {
public:
    std::vector<std::vector<int>> verticalTraversal(TreeNode* root) {
        std::vector<std::tuple<int, int, int>> nodes; // {col, row, val}
        dfs(root, 0, 0, nodes);

        std::sort(nodes.begin(), nodes.end());

        std::vector<std::vector<int>> out;
        int prev_col = INT_MIN;

        for (auto& [col, row, val] : nodes) {
            if (col != prev_col) {
                out.push_back({});
                prev_col = col;
            }
            out.back().push_back(val);
        }
        return out;
    }

private:
    void dfs(TreeNode* node, int row, int col,
             std::vector<std::tuple<int, int, int>>& nodes) {
        if (!node) { return; }
        nodes.push_back({col, row, node->val});
        dfs(node->left, row + 1, col - 1, nodes);
        dfs(node->right, row + 1, col + 1, nodes);
    }
};

// {col, row, val} tuple ordering: std::tuple sorts lexicographically, col first then row then val
// std::map alternative map {col -> sorted (row, val) pairs}, flat vector + sort is cache-friendly, contiguous memory, SIMD-sortable
// prev_col grouping: after sort, same-col entries are contiguous
