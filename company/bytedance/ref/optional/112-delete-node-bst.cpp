// ByteDance / LC 450: delete the node with value key from a BST (if present) and return
// the new root; the result must still be a BST.
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
};

// Recurse toward key; two children -> take the successor's value, delete the successor.
// Nodes are only unlinked, never freed (LeetCode does not free them either).
// T: O(h), S: O(h).
TreeNode* deleteNode(TreeNode* root, int key) {
    if (!root) { return nullptr; }
    if (key < root->val) {
        root->left = deleteNode(root->left, key);
    } else if (key > root->val) {
        root->right = deleteNode(root->right, key);
    } else {
        if (!root->left) { return root->right; }
        if (!root->right) { return root->left; }
        // successor = leftmost of the right subtree
        TreeNode* succ = root->right;
        while (succ->left) { succ = succ->left; }
        root->val = succ->val;
        root->right = deleteNode(root->right, succ->val);
    }
    return root;
}

void inorder(TreeNode* node, vector<int>& out) {
    if (!node) { return; }
    inorder(node->left, out);
    out.push_back(node->val);
    inorder(node->right, out);
}

void test(const string& label, TreeNode* root, int key, const vector<int>& want,
          int wantRoot) {
    TreeNode* res = deleteNode(root, key);
    vector<int> got;
    inorder(res, got);
    cout << label << (got == want && res->val == wantRoot ? " PASS\n" : " FAIL\n");
}

int main() {
    //     5
    //   3   6
    //  2 4    7
    TreeNode a2{2, nullptr, nullptr};
    TreeNode a4{4, nullptr, nullptr};
    TreeNode a7{7, nullptr, nullptr};
    TreeNode a3{3, &a2, &a4};
    TreeNode a6{6, nullptr, &a7};
    TreeNode a5{5, &a3, &a6};
    test("two-children", &a5, 3, {2, 4, 5, 6, 7}, 5);
    TreeNode b2{2, nullptr, nullptr};
    TreeNode b4{4, nullptr, nullptr};
    TreeNode b7{7, nullptr, nullptr};
    TreeNode b3{3, &b2, &b4};
    TreeNode b6{6, nullptr, &b7};
    TreeNode b5{5, &b3, &b6};
    test("absent", &b5, 0, {2, 3, 4, 5, 6, 7}, 5);
    test("root", &b5, 5, {2, 3, 4, 6, 7}, 6);
}

// follow-up: relink instead of copying val -> detach succ (its parent takes succ->right),
//   succ adopts root's left/right, return succ, delete root; same O(h)
// follow-up: iterative, O(1) space -> walk with parent pointer to key, splice as above
// follow-up: keep h = O(log n) -> self-balancing BST (AVL / red-black, as in set)
