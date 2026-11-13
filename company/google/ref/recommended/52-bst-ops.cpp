// 52 · Validate / Insert / Delete in a BST — LC 98, 701, 450
// Mother problem: select/07-trees/03-validate-bst.cpp
//
// Google reports: implement a BST (`insertIntoBST`, `deleteNode`)
//
// LC 98: true if every left subtree holds only smaller values and every right subtree
// only larger values (strict), at every node.
//
// Clarify
// - strict: left < node < right; values span INT_MIN..INT_MAX → long long bounds
// - empty tree → valid; insert: val not in the tree yet; delete: missing key → no-op
// - delete returns the new root (it may change)
//
// Core idea
// - Pass (lo, hi) bounds down: left gets (lo, val), right gets (val, hi); a deep
//   violation (6 under 2 under 5) is caught.
// - Delete with two children: copy the in-order successor's value, delete it from the
//   right subtree.
// - Validate T O(n), S O(h); insert / delete T O(h).
//
// Tests
// - valid 5(2, 8); deep violation 5(2(_, 6), 8); duplicate 2(2); INT_MIN root
// - delete a missing key, a one-child node, a two-children root, the only root
//
// Follow-ups
// - in-order check → in-order must be strictly increasing; compare with a prev pointer
//   (not a sentinel value, so INT_MIN stays valid)
// - insert (LC 701) → `insertIntoBST` below; delete (LC 450) → `deleteNode` below

#include <climits>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

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

// LC 701: walk down by comparison and hang a new leaf at the empty spot; assumes val is
// not in the tree. Nodes are heap-allocated: free with deleteNode / freeTree.
// T: O(h), S: O(h).
TreeNode* insertIntoBST(TreeNode* root, int val) {
    if (!root) { return new TreeNode(val); }
    if (val < root->val) {
        root->left = insertIntoBST(root->left, val);
    } else {
        root->right = insertIntoBST(root->right, val);
    }
    return root;
}

// LC 450: find key, returning each subtree's new root. 0 or 1 child: the child takes
// its place; 2 children: copy the in-order successor's value here, then delete the
// successor from the right subtree. Frees the removed node; missing key = no-op.
// T: O(h), S: O(h).
TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root) { return nullptr; }
    if (key < root->val) {
        root->left = deleteNode(root->left, key);
        return root;
    }
    if (key > root->val) {
        root->right = deleteNode(root->right, key);
        return root;
    }
    if (!root->left || !root->right) {
        TreeNode* child = root->left ? root->left : root->right;
        delete root;
        return child;
    }
    // successor = leftmost node of the right subtree; it has no left child
    TreeNode* succ = root->right;
    while (succ->left) {
        succ = succ->left;
    }
    root->val = succ->val;
    root->right = deleteNode(root->right, succ->val);
    return root;
}

// in-order values of a tree, for tests
void collectInorder(TreeNode* node, vector<int>& out) {
    if (!node) { return; }
    collectInorder(node->left, out);
    out.push_back(node->val);
    collectInorder(node->right, out);
}

// frees a tree built by insertIntoBST
void freeTree(TreeNode* node) {
    if (!node) { return; }
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

void test(const string& label, TreeNode* root, bool want) {
    cout << label << (isValidBST(root) == want ? " PASS\n" : " FAIL\n");
}

// inserts vals in order, deletes key, then checks the in-order values and validity
void testInsertDelete(const string& label, const vector<int>& vals, int key,
                      const vector<int>& want) {
    TreeNode* root = nullptr;
    for (int val : vals) {
        root = insertIntoBST(root, val);
    }
    root = deleteNode(root, key);
    vector<int> got;
    collectInorder(root, got);
    bool ok = got == want && isValidBST(root);
    freeTree(root);
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
    TreeNode e0(0);
    TreeNode e(INT_MIN, nullptr, &e0);
    test("int-min", &e, true);
    testInsertDelete("del-missing", {5, 3, 8}, 4, {3, 5, 8});
    testInsertDelete("del-one-child", {5, 3, 2}, 3, {2, 5});
    testInsertDelete("del-two-children", {5, 3, 8, 6, 7}, 5, {3, 6, 7, 8});
    testInsertDelete("del-only-root", {5}, 5, {});
}
