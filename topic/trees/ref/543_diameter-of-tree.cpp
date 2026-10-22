// LC 543 Diameter of Binary Tree: return the length, counted in edges, of the longest
// path between any two nodes of a binary tree (it may skip the root).
#include <iostream>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::max;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// Returns the height of node's subtree in nodes. The longest path bending at node has
// left height + right height edges, recorded in best.
int dfs(TreeNode* node, int& best) {
    if (node == nullptr) { return 0; }
    int left = dfs(node->left, best);
    int right = dfs(node->right, best);
    best = max(best, left + right);
    return 1 + max(left, right);
}

// Post-order heights with a global best. T: O(n), S: O(h).
int diameterOfBinaryTree(TreeNode* root) {
    int best = 0;   // longest path in edges seen so far
    dfs(root, best);
    return best;
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (diameterOfBinaryTree(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a4(4), a5(5), a2(2, &a4), a3(3, &a5), a1(1, &a2, &a3);
    test("through-root", &a1, 4);
    TreeNode b4(4), b6(6), b3(3, &b4), b5(5, nullptr, &b6), b2(2, &b3, &b5), b1(1, &b2);
    test("skip-root", &b1, 4);
    TreeNode single(7);
    test("single", &single, 0);
}
