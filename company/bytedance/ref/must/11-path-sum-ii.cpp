// ByteDance (hack2hire) / LC 113: return every root-to-leaf path of a binary tree whose
// node values sum to targetSum. Values may be negative.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
};

// path = values from the root down to node; remain = target minus the sum above node
void dfs(TreeNode* node, int remain, vector<int>& path, vector<vector<int>>& result) {
    if (!node) { return; }
    path.push_back(node->val);
    remain -= node->val;
    // only a leaf may end a path
    if (!node->left && !node->right && remain == 0) { result.push_back(path); }
    dfs(node->left, remain, path, result);
    dfs(node->right, remain, path, result);
    // backtrack before the caller tries its other child
    path.pop_back();
}

// DFS backtracking; each hit copies a path of length <= h. T: O(n*h), S: O(h) + output.
vector<vector<int>> pathSum(TreeNode* root, int targetSum) {
    vector<vector<int>> result;
    vector<int> path;
    dfs(root, targetSum, path, result);
    return result;
}

void test(const string& label, TreeNode* root, int targetSum,
          const vector<vector<int>>& want) {
    cout << label << (pathSum(root, targetSum) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    //         5
    //      4     8
    //    11    13  4
    //   7  2      5  1
    TreeNode n7{7};
    TreeNode n2{2};
    TreeNode n13{13};
    TreeNode n5{5};
    TreeNode n1{1};
    TreeNode n11{11, &n7, &n2};
    TreeNode n4b{4, &n5, &n1};
    TreeNode n4{4, &n11};
    TreeNode n8{8, &n13, &n4b};
    TreeNode root{5, &n4, &n8};
    test("two-paths", &root, 22, {{5, 4, 11, 2}, {5, 8, 4, 5}});

    TreeNode m3{-2};
    TreeNode m2{2, &m3};
    TreeNode m1{1, &m2};
    test("sum-before-leaf", &m1, 3, {});

    TreeNode one{1};
    test("single", &one, 1, {{1}});
}

// follow-up: exists only (LC 112) -> same DFS returning bool, no path vector, O(n)
// follow-up: any downward path (LC 437) -> prefix sums along the root path, O(n);
//   cur = sum root..node, prefixCount starts as {0: 1}
//   count += prefixCount[cur - target];
//   prefixCount[cur]++;
//   dfs(node->left, cur); dfs(node->right, cur);
//   prefixCount[cur]--;
// follow-up: no recursion -> iterative postorder (stack + prev pointer); the stack holds
//   the root -> node path, check sum at leaves
