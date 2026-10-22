// LC 104 Maximum Depth of Binary Tree: return the number of nodes on the longest path
// from the root down to a leaf.
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

// Post-order recursion: depth = 1 + deeper child. T: O(n), S: O(h).
int maxDepth(TreeNode* root) {
    if (root == nullptr) { return 0; }
    return 1 + max(maxDepth(root->left), maxDepth(root->right));
}

void test(const string& label, TreeNode* root, int want) {
    cout << label << (maxDepth(root) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode d(4), c(3, &d), b(2), a(1, &b, &c);
    test("right-deeper", &a, 3);
    TreeNode single(5);
    test("single", &single, 1);
    test("empty", nullptr, 0);
}
