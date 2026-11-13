// LC 236 Lowest Common Ancestor of a Binary Tree (Medium) — post-order return
// given nodes p, q of a binary tree, return their LCA (a node can be its own ancestor)
//
// clarify:
// p, q both in tree -> assume yes; else follow-up (LC 1644)
// p != q -> assume yes
// compare by pointer or value -> pointer
// BST? -> O(h) walk, follow-up (LC 235)
// parent pointers? -> follow-up (LC 1650)

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

// core idea: call returns p or q if subtree holds one, else null; both sides hit = LCA
// T: O(n), S: O(h)
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    // stop at p or q: if the other is below, this node is the LCA anyway
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
    return 0;
}

// follow-up:
// BST (LC 235) -> walk down: both smaller go left, both larger go right, else split = LCA
// parent pointers, no root (LC 1650) -> like list intersection: walk up, swap at root
// p or q may be missing (LC 1644) -> no early return; count found, answer only if both
// k nodes / lowest common manager (LC 1676) -> same post-order w/ a set of targets
// delete nodes, return forest (LC 1110) -> post-order returns new pointer or null
// LCA of deepest leaves (LC 1123) -> return (depth, lca); equal depths -> node itself
// many queries on one tree -> binary lifting: up[j][v] = 2^j-th ancestor, O(log n) each
