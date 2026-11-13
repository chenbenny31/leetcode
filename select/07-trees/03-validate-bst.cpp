// LC 98 Validate Binary Search Tree (Medium) — DFS with (lo, hi) bounds
// true if at every node: left subtree all smaller, right subtree all larger (strict)
//
// clarify:
// dup -> invalid
// empty tree -> valid
// values INT_MIN / INT_MAX -> long long bounds
// tree depth -> recursion O(h); skewed = O(n)

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

// node must lie strictly inside (lo, hi) set by its ancestors
bool dfs(TreeNode* node, long long lo, long long hi) {
    if (!node) { return true; }
    if (node->val <= lo || node->val >= hi) { return false; }
    return dfs(node->left, lo, node->val) && dfs(node->right, node->val, hi);
}

// core idea: pass open interval down; left tightens hi to node's val, right tightens lo
// T: O(n), S: O(h)
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

// follow-up:
// inorder version -> BST inorder strictly increasing; compare w/ prev node pointer
// insert into BST (LC 701) -> walk down by comparison, hang new leaf at the empty spot
// delete from BST (LC 450) -> 0 / 1 child: child takes its place; 2: copy successor
// recover BST w/ two swapped nodes (LC 99) -> inorder w/ prev; first drop + last drop
// largest BST subtree (LC 333) -> post-order returning (isBST, size, min, max)
// dup allowed on one side -> make that side's comparison non-strict
