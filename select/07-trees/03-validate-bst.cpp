// LC 98 Validate Binary Search Tree (Medium). Pattern: DFS with (lo, hi) bounds.
// True if every left subtree holds only smaller values and every right subtree only
// larger values (strict), at every node.

#include <climits>
#include <iostream>
#include <string>

using std::cout;
using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(x), left(l), right(r) {}
};

// each node must lie strictly inside (lo, hi) set by its ancestors
bool dfs(TreeNode* node, long long lo, long long hi) {
    if (!node) { return true; }
    if (node->val <= lo || node->val >= hi) { return false; }
    return dfs(node->left, lo, node->val) && dfs(node->right, node->val, hi);
}

// Pass bounds down; long long so INT_MIN / INT_MAX values stay valid. T: O(n), S: O(h).
bool isValidBST(TreeNode* root) {
    return dfs(root, LLONG_MIN, LLONG_MAX);
}

// inorder walk; prev = last visited node (nullptr before the first one)
bool inorder(TreeNode* cur, TreeNode*& prev) {
    if (!cur) { return true; }
    if (!inorder(cur->left, prev)) { return false; }
    if (prev && prev->val >= cur->val) { return false; }
    prev = cur;
    return inorder(cur->right, prev);
}

// Inorder of a BST is strictly increasing; compare each node with the previous one.
// A prev pointer (not a sentinel value) keeps INT_MIN valid. T: O(n), S: O(h).
bool isValidBSTInorder(TreeNode* root) {
    TreeNode* prev = nullptr;
    return inorder(root, prev);
}

void test(const string& label, TreeNode* root, bool want) {
    bool ok = isValidBST(root) == want && isValidBSTInorder(root) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a2(2);
    TreeNode a8(8);
    TreeNode a5(5, &a2, &a8);
    test("valid", &a5, true);
    // 5(2(_, 6), 8): 6 is right of 2 but left of 5
    TreeNode b6(6);
    TreeNode b2(2, nullptr, &b6);
    TreeNode b8(8);
    TreeNode b5(5, &b2, &b8);
    test("deep-violation", &b5, false);
    TreeNode c2(2);
    TreeNode c2b(2, &c2);
    test("duplicate", &c2b, false);
    TreeNode d(INT_MAX);
    test("int-max", &d, true);
    TreeNode e0(0);
    TreeNode e(INT_MIN, nullptr, &e0);
    test("int-min", &e, true);
}
