// LC 687 Longest Univalue Path: return the length, counted in edges, of the longest path
// in a binary tree where every node on the path has the same value.
#include <iostream>
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

// Returns the longest same-value path going down from node. A child extends the path
// only if it holds the same value; the path bending at node adds both sides into best.
int dfs(TreeNode* node, int& best) {
    if (node == nullptr) { return 0; }
    int left = dfs(node->left, best);
    int right = dfs(node->right, best);
    bool leftSame = node->left != nullptr && node->left->val == node->val;
    bool rightSame = node->right != nullptr && node->right->val == node->val;
    left = leftSame ? left + 1 : 0;
    right = rightSame ? right + 1 : 0;
    best = max(best, left + right);
    return max(left, right);
}

// Post-order DFS with a global best. T: O(n), S: O(h).
int longestUnivaluePath(TreeNode* root) {
    int best = 0;   // longest same-value path in edges seen so far
    dfs(root, best);
    return best;
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (longestUnivaluePath(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode c(4), d(4), b(4, &c, &d), a(1, &b);
    test("bend-at-node", &a, 2);
    TreeNode e(1), f(2, &e), g(1, &f), h(1, &g);
    test("value-gap", &h, 1);
    TreeNode single(3);
    test("single", &single, 0);
}
