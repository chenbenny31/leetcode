// LC 222 Count Complete Tree Nodes: count the nodes of a complete binary tree (every
// level full except possibly the last, which is filled from the left) in less than O(n).
#include <iostream>
#include <string>
using std::cout;
using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// If the leftmost and rightmost depths match, the subtree is perfect: 2^h - 1 nodes.
// Otherwise recurse; one child is always perfect, so only one side goes deep.
// T: O(log^2 n), S: O(log n).
int countNodes(TreeNode* root) {
    int leftDepth = 0;
    int rightDepth = 0;
    for (TreeNode* cur = root; cur != nullptr; cur = cur->left) {
        leftDepth++;
    }
    for (TreeNode* cur = root; cur != nullptr; cur = cur->right) {
        rightDepth++;
    }
    if (leftDepth == rightDepth) { return (1 << leftDepth) - 1; }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (countNodes(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n4(4), n5(5), n6(6), n2(2, &n4, &n5), n3(3, &n6), n1(1, &n2, &n3);
    test("last-level-partial", &n1, 6);
    TreeNode p4(4), p5(5), p6(6), p7(7);
    TreeNode p2(2, &p4, &p5), p3(3, &p6, &p7), p1(1, &p2, &p3);
    test("perfect", &p1, 7);
    TreeNode o2(2), o1(1, &o2);
    test("one-child", &o1, 2);
    test("empty", nullptr, 0);
}
