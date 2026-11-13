// 51 · Lowest Common Ancestor of a Binary Tree — LC 236, 1676, 1110
// Mother problem: select/07-trees/02-lca.cpp
//
// Google reports: cheapest common ancestor, blood relation, lowest common manager of k
// ×2, delete nodes into a forest ×2 (`delNodes`), pedigree ×2
//
// LC 236: given two nodes p and q of a binary tree, return their lowest common ancestor
// (a node may be its own ancestor).
//
// Clarify
// - p and q are both in the tree → assume yes; p may be the LCA
// - given node pointers, not values; values may repeat
//
// Core idea
// - Post-order: each call returns p or q if its subtree holds one, else null.
// - Reaching p or q stops the descent; the first node with hits from both sides is the
//   LCA. T O(n), S O(h).
//
// Tests
// - p and q in different subtrees → root; p ancestor of q → p; sibling leaves → parent
//
// Follow-ups
// - BST (LC 235) → walk down while both are on one side; the split node is the LCA, O(h)
// - parent pointers (LC 1650) → walk up from p and q, a pointer past the root restarts
//   at the other node (like list intersection); they meet at the LCA
// - LCA of k nodes / lowest common manager (LC 1676) → same post-order, stop on any node
//   in a target set
// - delete nodes and return the forest (LC 1110) → `delNodes` below

#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_set;

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

// Returns node's new pointer for its parent (null if node is deleted); a kept node
// whose parent is gone (or missing) starts a tree of the forest.
TreeNode* dfs(TreeNode* node, bool parentGone, const unordered_set<int>& toDelete,
              vector<TreeNode*>& forest) {
    if (!node) { return nullptr; }
    bool deleted = toDelete.count(node->val) > 0;
    if (parentGone && !deleted) { forest.push_back(node); }
    node->left = dfs(node->left, deleted, toDelete, forest);
    node->right = dfs(node->right, deleted, toDelete, forest);
    return deleted ? nullptr : node;
}

// LC 1110 Delete Nodes And Return Forest: values are distinct; deleted nodes are only
// unlinked (nodes here are not heap-owned). Roots come out in pre-order.
// T: O(n), S: O(h + d).
vector<TreeNode*> delNodes(TreeNode* root, const unordered_set<int>& toDelete) {
    vector<TreeNode*> forest;
    dfs(root, true, toDelete, forest);
    return forest;
}

// pre-order values of a tree, to compare forests in tests
void preorder(TreeNode* node, vector<int>& out) {
    if (!node) { return; }
    out.push_back(node->val);
    preorder(node->left, out);
    preorder(node->right, out);
}

void test(const string& label, TreeNode* root, TreeNode* p, TreeNode* q, TreeNode* want) {
    cout << label << (lowestCommonAncestor(root, p, q) == want ? " PASS\n" : " FAIL\n");
}

// builds a fresh 1(2(4, 5), 3(6, 7)) each call since delNodes unlinks nodes;
// want = pre-order values of each forest tree, in order
void testDelete(const string& label, const unordered_set<int>& toDelete,
                const vector<vector<int>>& want) {
    TreeNode n4(4);
    TreeNode n5(5);
    TreeNode n6(6);
    TreeNode n7(7);
    TreeNode n2(2, &n4, &n5);
    TreeNode n3(3, &n6, &n7);
    TreeNode n1(1, &n2, &n3);
    vector<vector<int>> got;
    for (TreeNode* tree : delNodes(&n1, toDelete)) {
        vector<int> vals;
        preorder(tree, vals);
        got.push_back(vals);
    }
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
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
    testDelete("del-root", {1}, {{2, 4, 5}, {3, 6, 7}});
    testDelete("del-inner", {2}, {{1, 3, 6, 7}, {4}, {5}});
    testDelete("del-parent-and-leaf", {3, 7}, {{1, 2, 4, 5}, {6}});
    testDelete("del-none", {}, {{1, 2, 4, 5, 3, 6, 7}});
}
