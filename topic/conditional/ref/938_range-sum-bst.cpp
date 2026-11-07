// LC 938 Range Sum of BST: sum the values of the BST nodes in the inclusive range
// [low, high].
#include <iostream>
#include <string>

using std::cout;
using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// DFS that prunes by the BST order: a node below low has nothing useful on its left,
// a node above high has nothing useful on its right. T: O(n), S: O(h).
int rangeSumBST(TreeNode* root, int low, int high) {
    if (!root) { return 0; }
    if (root->val < low) { return rangeSumBST(root->right, low, high); }
    if (root->val > high) { return rangeSumBST(root->left, low, high); }
    int left = rangeSumBST(root->left, low, high);
    int right = rangeSumBST(root->right, low, high);
    return root->val + left + right;
}

void test(const string& label, TreeNode* root, int low, int high, int want) {
    cout << label << (rangeSumBST(root, low, high) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n10(10), n5(5), n15(15), n3(3), n7(7), n18(18);
    n10.left = &n5;
    n10.right = &n15;
    n5.left = &n3;
    n5.right = &n7;
    n15.right = &n18;

    test("middle-range", &n10, 7, 15, 32);
    test("whole-tree", &n10, 0, 100, 58);
    test("no-match", &n10, 11, 14, 0);
    test("empty", nullptr, 1, 2, 0);
}
