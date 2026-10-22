// LC 450 Delete Node in a BST: delete the node with the given key from a binary search
// tree (if present) and return the root of the resulting valid BST.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// Search down by key. A node with at most one child is replaced by that child; a node
// with two children takes its inorder successor's value, then the successor is deleted
// from the right subtree. Unlinked nodes are not freed here (the caller owns them).
// T: O(h), S: O(h).
TreeNode* deleteNode(TreeNode* root, int key) {
    if (root == nullptr) { return nullptr; }
    if (key < root->val) {
        root->left = deleteNode(root->left, key);
        return root;
    }
    if (key > root->val) {
        root->right = deleteNode(root->right, key);
        return root;
    }
    if (root->left == nullptr) { return root->right; }
    if (root->right == nullptr) { return root->left; }
    TreeNode* succ = root->right;   // smallest value in the right subtree
    while (succ->left != nullptr) {
        succ = succ->left;
    }
    root->val = succ->val;
    root->right = deleteNode(root->right, succ->val);
    return root;
}

void inorder(TreeNode* root, vector<int>& out) {
    if (root == nullptr) { return; }
    inorder(root->left, out);
    out.push_back(root->val);
    inorder(root->right, out);
}

// An ascending inorder is exactly the BST property, so it checks validity too.
void test(const string& label, int key, const vector<int>& want) {
    TreeNode n2(2), n4(4), n7(7);
    TreeNode n3(3, &n2, &n4), n6(6, nullptr, &n7), n5(5, &n3, &n6);
    vector<int> got;
    inorder(deleteNode(&n5, key), got);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("leaf", 2, {3, 4, 5, 6, 7});
    test("one-child", 6, {2, 3, 4, 5, 7});
    test("root-two-children", 5, {2, 3, 4, 6, 7});
    test("missing", 9, {2, 3, 4, 5, 6, 7});
}
