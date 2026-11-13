// LC 543 Diameter of Binary Tree (Easy) — post-order height, update answer per node
// return num of edges on the longest path between any two nodes
//
// clarify:
// len in edges or nodes -> edges
// empty tree -> 0
// path must pass root? -> no
// tree depth -> recursion stack O(h)

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
    TreeNode(int x, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(x), left(l), right(r) {}
};

// returns height of node's subtree in nodes (0 for null)
int dfs(TreeNode* node, int& diameter) {
    if (!node) { return 0; }
    int leftHeight = dfs(node->left, diameter);
    int rightHeight = dfs(node->right, diameter);
    // edges of longest path whose highest node is this one
    diameter = max(diameter, leftHeight + rightHeight);
    return 1 + max(leftHeight, rightHeight);
}

// core idea: longest path bending at a node = leftHeight + rightHeight edges; keep best
// T: O(n), S: O(h)
int diameterOfBinaryTree(TreeNode* root) {
    int diameter = 0;
    dfs(root, diameter);
    return diameter;
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (diameterOfBinaryTree(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n4(4);
    TreeNode n5(5);
    TreeNode n2(2, &n4, &n5);
    TreeNode n3(3);
    TreeNode n1(1, &n2, &n3);
    test("through-root", &n1, 3);
    TreeNode b5(5);
    TreeNode b6(6);
    TreeNode b3(3, &b5, nullptr);
    TreeNode b4(4, nullptr, &b6);
    TreeNode b2(2, &b3, &b4);
    TreeNode b1(1, &b2, nullptr);
    test("not-through-root", &b1, 4);
    TreeNode single(1);
    test("single-node", &single, 0);
    test("empty", nullptr, 0);
    return 0;
}

// follow-up:
// max path sum (LC 124) -> dfs returns max(0, val + better child gain); ans = val + L + R
// longest same-value path (LC 687) -> arm = child arm + 1 if child val == val, else 0
// longest consecutive path (LC 298) -> run = child run + 1 if child = val + 1, else 1
// balanced tree (LC 110) -> dfs returns height, or -1 = unbalanced, passed straight up
// n-ary tree diameter -> keep top two child heights per node; ans = max sum of the two
// general tree as graph -> BFS from any node to farthest a, then BFS from a
// sum of distances (LC 834) -> reroot: ans[child] = ans[parent] - size + (n - size)
// subtree of another tree (LC 572) -> same-tree check at every node, O(m * n)
