// LC 124 Binary Tree Maximum Path Sum: a path is any chain of connected nodes (it need
// not pass through the root). Return the maximum sum of node values over all paths.
#include <iostream>
#include <climits>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::max;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// Returns the best sum of a path that starts at node and goes down one side. A path that
// bends at node uses both sides, so that candidate is recorded in best. Negative gains
// are dropped by clamping at 0.
int dfs(TreeNode* node, int& best) {
    if (node == nullptr) { return 0; }
    int left = max(0, dfs(node->left, best));
    int right = max(0, dfs(node->right, best));
    best = max(best, node->val + left + right);
    return node->val + max(left, right);
}

// Post-order DFS with a global best. T: O(n), S: O(h).
int maxPathSum(TreeNode* root) {
    int best = INT_MIN;   // best path sum seen so far
    dfs(root, best);
    return best;
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (maxPathSum(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a2(2), a3(3), a1(1, &a2, &a3);
    test("through-root", &a1, 6);
    TreeNode b3(3), bm1(-1), b6(6, &b3, &bm1), b4(4), bm5(-5, &b4, &b6);
    test("skip-root", &bm5, 9);
    TreeNode cm1(-1), cm2(-2), cm3(-3, &cm1, &cm2);
    test("all-negative", &cm3, -1);
}
