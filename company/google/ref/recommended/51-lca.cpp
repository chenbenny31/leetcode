// 51 · Lowest Common Ancestor of a Binary Tree — LC 236, 1676, 1110
// Mother: select/07-trees/02-lca.cpp
// (1) LC 236: lowest common ancestor of p and q; a node may be its own ancestor
// (2) LC 1110: delete nodes w/ val in toDelete; return the forest's roots
// reported: common ancestor / manager of k ×2, pedigree ×2, delete -> forest ×2
//
// clarify:
// p, q both in the tree -> assumed yes
// node ptrs or values -> ptrs; values may repeat
// p is an ancestor of q -> answer = p
// (2) values distinct; order of roots -> pre-order

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

// (1) LCA
// core idea: post-order; return p / q if subtree holds one; node w/ both sides = LCA
// T: O(n), S: O(h)
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    // stop at p / q: if the other one is below, this node is the LCA anyway
    if (!root || root == p || root == q) { return root; }
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);
    if (left && right) { return root; }
    return left ? left : right;
}

// returns node's new ptr for its parent (null if deleted)
TreeNode* dfs(TreeNode* node, bool parentGone, const unordered_set<int>& toDelete,
              vector<TreeNode*>& forest) {
    if (!node) { return nullptr; }
    bool deleted = toDelete.count(node->val) > 0;
    if (parentGone && !deleted) { forest.push_back(node); }
    node->left = dfs(node->left, deleted, toDelete, forest);
    node->right = dfs(node->right, deleted, toDelete, forest);
    return deleted ? nullptr : node;
}

// (2) delete nodes -> forest
// core idea: dfs returns node's new ptr to parent; kept node w/ parent gone = new root
// T: O(n), S: O(h + d)
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

// fresh 1(2(4, 5), 3(6, 7)) each call; want = pre-order values of each forest tree
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

// follow-up:
// BST (LC 235) -> walk down while both on one side; split node = LCA, O(h)
// parent ptrs (LC 1650) -> walk up from p and q; past root, restart at the other node
// LCA of k nodes / lowest common manager (LC 1676) -> same post-order, stop on any target
