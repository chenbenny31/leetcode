// LC 98 Validate Binary Search Tree (Medium) — DFS with (lo, hi) bounds
// True if every left subtree holds only smaller values and every right subtree only
// larger values (strict), at every node.
// Clarify: duplicates make it invalid; empty tree is valid; values can be INT_MIN /
//   INT_MAX; tree depth (recursion).

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

// Core idea: pass an open interval down; going left tightens hi to the node's value,
// going right tightens lo. long long bounds keep INT_MIN / INT_MAX values valid.
// T: O(n), S: O(h).
bool isValidBST(TreeNode* root) {
    return dfs(root, LLONG_MIN, LLONG_MAX);
}

void test(const string& label, TreeNode* root, bool want) {
    cout << label << (isValidBST(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a2(2);
    TreeNode a8(8);
    TreeNode a5(5, &a2, &a8);
    test("valid", &a5, true);
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
    return 0;
}

// follow-up (ideas only):
// - inorder version -> inorder of a BST is strictly increasing; compare each node with a
//   prev node pointer (not a sentinel value, so INT_MIN stays valid)
// - insert into a BST (LC 701) -> walk down by comparison and hang a new leaf at the
//   empty spot; the recursive form returns each subtree's root
// - delete from a BST (LC 450) -> 0 or 1 child: the child takes its place; 2 children:
//   copy the in-order successor's value here, then delete it from the right subtree
// - recover a BST with two swapped nodes (LC 99) -> inorder with prev; the first drop
//   gives the first node, the last drop the second; swap their values
// - largest BST subtree (LC 333) -> post-order returning (isBST, size, min, max)
// - duplicates allowed on one side -> make that side's comparison non-strict
