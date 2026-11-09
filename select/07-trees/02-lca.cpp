// LC 236 Lowest Common Ancestor of a Binary Tree (Medium). Pattern: post-order return.
// Given two nodes p and q of a binary tree, return their lowest common ancestor
// (a node may be its own ancestor).

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

// Each call returns p or q if its subtree holds one, else null; the first node that
// gets hits from both sides is the LCA. T: O(n), S: O(h).
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    // reaching p or q stops the descent: the other one below it makes it the LCA anyway
    if (!root || root == p || root == q) { return root; }
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);
    if (left && right) { return root; }
    return left ? left : right;
}

void test(const string& label, TreeNode* root, TreeNode* p, TreeNode* q, TreeNode* want) {
    cout << label << (lowestCommonAncestor(root, p, q) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    // 1(2(4, 5), 3(6, 7))
    TreeNode n4(4);
    TreeNode n5(5);
    TreeNode n6(6);
    TreeNode n7(7);
    TreeNode n2(2, &n4, &n5);
    TreeNode n3(3, &n6, &n7);
    TreeNode n1(1, &n2, &n3);
    test("split-at-root", &n1, &n4, &n7, &n1);
    test("p-is-ancestor", &n1, &n2, &n5, &n2);
    test("sibling-leaves", &n1, &n6, &n7, &n3);
}

// follow-up: BST (LC 235) -> from the root, both smaller go left, both larger go right,
//   else the current node is the LCA, O(h)
// follow-up: nodes have parent pointers (LC 1650) -> two pointers as in list intersection
