// ByteDance (hack2hire) / LC 337: houses form a binary tree; robbing two directly
// linked houses alerts the police. Return the max amount robbed.
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>

using std::cout;
using std::string;
using std::pair;
using std::max;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// returns {best if node is robbed, best if node is skipped} for node's subtree
pair<int, int> dfs(TreeNode* node) {
    if (!node) { return {0, 0}; }
    pair<int, int> l = dfs(node->left);
    pair<int, int> r = dfs(node->right);
    // robbing node forces both children to be skipped
    int rob = node->val + l.second + r.second;
    int skip = max(l.first, l.second) + max(r.first, r.second);
    return {rob, skip};
}

// Postorder tree DP with (rob, skip) per node. T: O(n), S: O(h).
int rob(TreeNode* root) {
    pair<int, int> res = dfs(root);
    return max(res.first, res.second);
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (rob(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    //     3
    //   2   3
    //    3    1
    TreeNode a4{3, nullptr, nullptr};
    TreeNode a5{1, nullptr, nullptr};
    TreeNode a2{2, nullptr, &a4};
    TreeNode a3{3, nullptr, &a5};
    TreeNode a1{3, &a2, &a3};
    test("rob-grandkids", &a1, 7);
    //     3
    //   4   5
    //  1 3    1
    TreeNode b4{1, nullptr, nullptr};
    TreeNode b5{3, nullptr, nullptr};
    TreeNode b6{1, nullptr, nullptr};
    TreeNode b2{4, &b4, &b5};
    TreeNode b3{5, nullptr, &b6};
    TreeNode b1{3, &b2, &b3};
    test("rob-kids", &b1, 9);
    test("empty", nullptr, 0);
}

// follow-up: which houses -> keep (rob, skip) per node in a map; second pass from root:
//   robbed parent forces children skip, else child robs if rob > skip
// follow-up: array / circle (LC 198, 213) -> rob = prevSkip + x, skip = max(prevRob,
//   prevSkip); circle = max(range without first, range without last)
// follow-up: deep tree -> iterative postorder with a stack + unordered_map<TreeNode*,
//   pair<int, int>> dp
