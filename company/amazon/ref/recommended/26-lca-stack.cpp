// LCA with a stack: given a binary tree with unique values and two values p, q in it,
// return the value of their lowest common ancestor. Must be iterative: walk the tree
// with a stack to build parent links, then climb from p and q.

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

// Stack DFS records every node's parent; put all of p's ancestors in a set, then climb
// from q until the first node in that set. T: O(n), S: O(n).
int lowestCommonAncestor(TreeNode* root, int p, int q) {
    // parent[v] = parent of the node with value v (root -> nullptr)
    unordered_map<int, TreeNode*> parent;
    // node[v] = the node with value v
    unordered_map<int, TreeNode*> node;
    parent[root->val] = nullptr;
    vector<TreeNode*> stk{root};
    while (!stk.empty()) {
        TreeNode* cur = stk.back();
        stk.pop_back();
        node[cur->val] = cur;
        if (cur->left != nullptr) {
            parent[cur->left->val] = cur;
            stk.push_back(cur->left);
        }
        if (cur->right != nullptr) {
            parent[cur->right->val] = cur;
            stk.push_back(cur->right);
        }
    }
    // p and every ancestor of p
    unordered_set<TreeNode*> ancestors;
    for (TreeNode* a = node[p]; a != nullptr; a = parent[a->val]) { ancestors.insert(a); }
    TreeNode* b = node[q];
    while (!ancestors.count(b)) { b = parent[b->val]; }
    return b->val;
}

void test(const string& label, TreeNode* root, int p, int q, int want) {
    cout << label << (lowestCommonAncestor(root, p, q) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    // 3 has children 5, 1; 5 has children 6, 2
    TreeNode a(3), b(5), c(1), d(6), e(2);
    a.left = &b;
    a.right = &c;
    b.left = &d;
    b.right = &e;
    test("split-at-root", &a, 6, 1, 3);
    test("ancestor-of-other", &a, 5, 2, 5);
    test("siblings", &a, 6, 2, 5);
}
