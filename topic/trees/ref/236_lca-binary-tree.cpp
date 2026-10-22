// LC 236 Lowest Common Ancestor of a Binary Tree: given a binary tree and two of its
// nodes p and q, return their lowest common ancestor (a node may be its own ancestor).
#include <iostream>
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

// Post-order: each call returns p or q if its subtree holds one, else nullptr. A node
// that gets a hit from both sides is the LCA; hitting p or q itself stops the descent.
// T: O(n), S: O(h).
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if (root == nullptr || root == p || root == q) { return root; }
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);
    if (left != nullptr && right != nullptr) { return root; }
    return left != nullptr ? left : right;
}

void test(const string& label, TreeNode* root, TreeNode* p, TreeNode* q, TreeNode* want) {
    cout << label << (lowestCommonAncestor(root, p, q) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n4(4), n5(5), n6(6), n7(7);
    TreeNode n2(2, &n4, &n5), n3(3, &n6, &n7), n1(1, &n2, &n3);
    test("split-at-root", &n1, &n4, &n7, &n1);
    test("p-is-ancestor", &n1, &n2, &n5, &n2);
    test("sibling-leaves", &n1, &n6, &n7, &n3);
}
