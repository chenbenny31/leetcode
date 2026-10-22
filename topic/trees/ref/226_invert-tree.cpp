// LC 226 Invert Binary Tree: mirror a binary tree by swapping the left and right child
// of every node, and return its root.
#include <iostream>
#include <string>
#include <utility>
using std::cout;
using std::string;
using std::swap;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// Recurse into both children, then swap them. T: O(n), S: O(h).
TreeNode* invertTree(TreeNode* root) {
    if (root == nullptr) { return nullptr; }
    invertTree(root->left);
    invertTree(root->right);
    swap(root->left, root->right);
    return root;
}

bool sameTree(TreeNode* a, TreeNode* b) {
    if (a == nullptr || b == nullptr) { return a == b; }
    return a->val == b->val && sameTree(a->left, b->left) && sameTree(a->right, b->right);
}

void test(const string& label, TreeNode* root, TreeNode* want) {
    cout << label << (sameTree(invertTree(root), want) ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a3(3), a2(2, &a3), a1(1, &a2);
    TreeNode w3(3), w2(2, nullptr, &w3), w1(1, nullptr, &w2);
    test("left-chain", &a1, &w1);
    TreeNode b4(4), b5(5), b2(2, &b4), b3(3, nullptr, &b5), b1(1, &b2, &b3);
    TreeNode x4(4), x5(5), x2(2, nullptr, &x4), x3(3, &x5), x1(1, &x3, &x2);
    test("both-sides", &b1, &x1);
    test("empty", nullptr, nullptr);
}
