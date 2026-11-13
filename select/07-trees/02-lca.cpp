// LC 236 Lowest Common Ancestor of a Binary Tree (Medium) — post-order return
// Given two nodes p and q of a binary tree, return their lowest common ancestor
// (a node may be its own ancestor).
// Clarify: p and q both in the tree; p != q; nodes compared by pointer, not value; is
//   the tree a BST; parent pointers available.

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

// Core idea: each call returns p or q if its subtree holds one, else null; the first node
// that gets a hit from both sides is the LCA. T: O(n), S: O(h).
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

// follow-up (ideas only):
// - BST (LC 235) -> walk down from the root: both smaller go left, both larger go right,
//   otherwise this node is where p and q split; O(h) time, O(1) space
// - parent pointers, no root (LC 1650) -> like list intersection: walk up from p and q, a
//   pointer that passes the root restarts at the other node; they meet at the LCA
// - p or q may be missing (LC 1644) -> no early return at p / q: finish the post-order,
//   count the nodes found and answer only when both were seen
// - k nodes / lowest common manager (LC 1676) -> same post-order with a set of targets;
//   any target stops the descent
// - delete nodes, return the forest (LC 1110) -> post-order returning each node's new
//   pointer (null if deleted); a kept node whose parent is gone starts a new tree
// - LCA of the deepest leaves (LC 1123) -> return (depth, lca) per subtree; equal depths
//   on both sides make the node itself the answer
// - many queries on one tree -> binary lifting: up[j][v] = 2^j-th ancestor, O(log n) each
