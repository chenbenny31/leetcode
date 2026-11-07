// LC 236 (Amazon onsite "LCA with stack"): given a binary tree with unique values and
// two values p, q in it, return their lowest common ancestor (a node counts as its own
// ancestor).
// Follow-up: no recursion -> a stack DFS records parents, then climb from p and q.
// Follow-up: p or q may be missing -> nullptr. LCA of k values. Distance from p to q.
// Follow-up: parent pointers given -> recommended 64; a BST -> recommended 31.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;

struct TreeNode {
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode(int v) : val(v) {}
};

// Post-order: return the node holding p or q if this subtree has one, else nullptr.
// A node with hits on both sides is the LCA; reaching p or q stops the descent.
// T: O(n), S: O(h).
TreeNode* lowestCommonAncestor(TreeNode* root, int p, int q) {
    if (root == nullptr || root->val == p || root->val == q) { return root; }
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);
    if (left != nullptr && right != nullptr) { return root; }
    return left != nullptr ? left : right;
}

// follow-up: no recursion (a skewed tree can overflow the call stack). A stack DFS
// records every node's parent and finds p and q; put p and all its ancestors in a set,
// then climb from q to the first node in that set. T: O(n), S: O(n).
TreeNode* lcaIterative(TreeNode* root, int p, int q) {
    // parent[node] = its parent (root -> nullptr)
    unordered_map<TreeNode*, TreeNode*> parent;
    parent[root] = nullptr;
    TreeNode* pNode = nullptr;
    TreeNode* qNode = nullptr;
    vector<TreeNode*> stk{root};
    while (!stk.empty()) {
        TreeNode* cur = stk.back();
        stk.pop_back();
        if (cur->val == p) { pNode = cur; }
        if (cur->val == q) { qNode = cur; }
        if (cur->left != nullptr) {
            parent[cur->left] = cur;
            stk.push_back(cur->left);
        }
        if (cur->right != nullptr) {
            parent[cur->right] = cur;
            stk.push_back(cur->right);
        }
    }
    // p and every ancestor of p
    unordered_set<TreeNode*> ancestors;
    for (TreeNode* a = pNode; a != nullptr; a = parent[a]) { ancestors.insert(a); }
    TreeNode* b = qNode;
    while (!ancestors.count(b)) { b = parent[b]; }
    return b;
}

// follow-up: p or q may be missing. Same post-order, but children first so the whole
// tree is seen; found counts the nodes holding p or q. T: O(n), S: O(h).
TreeNode* findLcaCounting(TreeNode* root, int p, int q, int& found) {
    if (root == nullptr) { return nullptr; }
    // recurse before checking root, or a q below p would never be counted
    TreeNode* left = findLcaCounting(root->left, p, q, found);
    TreeNode* right = findLcaCounting(root->right, p, q, found);
    if (root->val == p || root->val == q) {
        found++;
        return root;
    }
    if (left != nullptr && right != nullptr) { return root; }
    return left != nullptr ? left : right;
}

// LCA if both p and q are in the tree, else nullptr. T: O(n), S: O(h).
TreeNode* lcaIfPresent(TreeNode* root, int p, int q) {
    int found = 0;
    TreeNode* lca = findLcaCounting(root, p, q, found);
    // p == q is a single node, so only one hit is expected
    int need = p == q ? 1 : 2;
    return found == need ? lca : nullptr;
}

// follow-up: LCA of a set of values, all in the tree. Same post-order, with a set
// lookup in place of the two comparisons. T: O(n), S: O(h).
TreeNode* lcaOfSet(TreeNode* root, const unordered_set<int>& targets) {
    if (root == nullptr || targets.count(root->val)) { return root; }
    TreeNode* left = lcaOfSet(root->left, targets);
    TreeNode* right = lcaOfSet(root->right, targets);
    if (left != nullptr && right != nullptr) { return root; }
    return left != nullptr ? left : right;
}

// Depth of value v below root (root is depth d), or -1 if it is not there.
// T: O(n), S: O(h).
int depthOf(TreeNode* root, int v, int d) {
    if (root == nullptr) { return -1; }
    if (root->val == v) { return d; }
    int left = depthOf(root->left, v, d + 1);
    if (left != -1) { return left; }
    return depthOf(root->right, v, d + 1);
}

// follow-up: edges between p and q = depth of p plus depth of q, both measured from
// their LCA. T: O(n), S: O(h).
int distance(TreeNode* root, int p, int q) {
    TreeNode* lca = lowestCommonAncestor(root, p, q);
    return depthOf(lca, p, 0) + depthOf(lca, q, 0);
}

void test(const string& label, TreeNode* root, int p, int q, int want) {
    bool ok = lowestCommonAncestor(root, p, q)->val == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a(3), b(5), c(1), d(6), e(2);
    a.left = &b;
    a.right = &c;
    b.left = &d;
    b.right = &e;
    test("split-at-root", &a, 6, 1, 3);
    test("ancestor-of-other", &a, 5, 2, 5);
    test("siblings", &a, 6, 2, 5);
}
