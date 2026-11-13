// LC 543 Diameter of Binary Tree (Easy) — post-order returns height, answer updated at
//   each node
// Return the number of edges on the longest path between any two nodes of the tree.
// Clarify: length in edges or in nodes; empty tree; the path need not pass the root;
//   tree depth (recursion stack).

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

// Returns the height of node's subtree in nodes (0 for null).
int dfs(TreeNode* node, int& diameter) {
    if (!node) { return 0; }
    int leftHeight = dfs(node->left, diameter);
    int rightHeight = dfs(node->right, diameter);
    // edges of the longest path whose highest node is this one
    diameter = max(diameter, leftHeight + rightHeight);
    return 1 + max(leftHeight, rightHeight);
}

// Core idea: the longest path that bends at a node has leftHeight + rightHeight edges;
// one post-order pass returns heights and keeps the best bend. T: O(n), S: O(h).
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

// follow-up (ideas only):
// - max path sum (LC 124) -> dfs returns the best downward gain max(0, val + better
//   child gain); answer = max over nodes of val + leftGain + rightGain
// - longest same-value path (LC 687) -> same shape; an arm is child arm + 1 only when the
//   child's value equals the node's, else it resets to 0
// - longest consecutive path (LC 298) -> dfs returns the run going down from the node:
//   child run + 1 when child.val == val + 1, else 1; answer = the largest run
// - balanced tree (LC 110) -> dfs returns the height, or -1 as "unbalanced" which is
//   passed straight up
// - n-ary tree diameter -> keep the top two child heights at each node; answer = the
//   largest sum of the two
// - diameter of a general tree given as a graph -> BFS from any node to the farthest node
//   a, then BFS from a; the farthest distance is the diameter
// - sum of distances to all nodes (LC 834) -> rerooting: post-order for subtree sizes and
//   the root's answer, then pre-order: answer[child] = answer[parent] - size + (n - size)
// - max depth (LC 104), invert (LC 226), same tree (LC 100) -> the same post-order
//   recursion returning a value (height / new root / bool)
// - subtree of another tree (LC 572) -> same-tree check at every node, O(m * n); or
//   compare serializations with null markers
// - count good nodes (LC 1448) -> pre-order: pass the max so far down the path, a node
//   counts when val >= that max
