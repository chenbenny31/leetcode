// LC 98 Validate Binary Search Tree: is the binary tree a valid BST, i.e. all values in
// a left subtree strictly less and in a right subtree strictly greater than its node?
#include <iostream>
#include <climits>
#include <string>
using std::cout;
using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// Bounds propagation: each node must lie strictly inside (lo, hi) set by its ancestors.
// long long bounds so INT_MIN / INT_MAX values stay valid. T: O(n), S: O(h).
bool dfs(TreeNode* node, long long lo, long long hi) {
    if (node == nullptr) { return true; }
    if (node->val <= lo || node->val >= hi) { return false; }
    return dfs(node->left, lo, node->val) && dfs(node->right, node->val, hi);
}

bool isValidBST(TreeNode* root) {
    return dfs(root, LLONG_MIN, LLONG_MAX);
}

void test(const string& label, TreeNode* root, bool want) {
    cout << label << (isValidBST(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a2(2), a8(8), a5(5, &a2, &a8);
    test("valid", &a5, true);

    TreeNode b6(6), b2(2, nullptr, &b6), b8(8), b5(5, &b2, &b8);
    test("deep-violation", &b5, false);

    TreeNode c2(2), c2b(2, &c2);
    test("duplicate", &c2b, false);

    TreeNode d(INT_MAX);
    test("int-max", &d, true);
}
