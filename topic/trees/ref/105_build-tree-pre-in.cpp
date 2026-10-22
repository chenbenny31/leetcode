// LC 105 Construct Binary Tree from Preorder and Inorder Traversal: given the preorder
// and inorder traversals of a binary tree with unique values, rebuild the tree.
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// Builds the subtree whose inorder range is [lo, hi). The next preorder value is its
// root; its inorder index splits the range. Left must be built before right.
TreeNode* build(const vector<int>& preorder, const unordered_map<int, int>& inIndex,
                int& pre, int lo, int hi) {
    if (lo >= hi) { return nullptr; }
    TreeNode* node = new TreeNode(preorder[pre++]);
    int mid = inIndex.at(node->val);
    node->left = build(preorder, inIndex, pre, lo, mid);
    node->right = build(preorder, inIndex, pre, mid + 1, hi);
    return node;
}

// Preorder gives each root, inorder splits left from right; a map makes the split O(1).
// T: O(n), S: O(n).
TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
    int n = inorder.size();
    unordered_map<int, int> inIndex;   // value -> its index in inorder
    for (int i = 0; i < n; i++) {
        inIndex[inorder[i]] = i;
    }
    int pre = 0;   // next unused position in preorder
    return build(preorder, inIndex, pre, 0, n);
}

bool sameTree(TreeNode* a, TreeNode* b) {
    if (a == nullptr || b == nullptr) { return a == b; }
    return a->val == b->val && sameTree(a->left, b->left) && sameTree(a->right, b->right);
}

void freeTree(TreeNode* root) {
    if (root == nullptr) { return; }
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

void test(const string& label, vector<int> pre, vector<int> in, TreeNode* want) {
    TreeNode* got = buildTree(pre, in);
    cout << label << (sameTree(got, want) ? " PASS\n" : " FAIL\n");
    freeTree(got);
}

int main() {
    TreeNode n4(4), n3(3), n2(2, &n4), n1(1, &n2, &n3);
    test("mixed", {1, 2, 4, 3}, {4, 2, 1, 3}, &n1);
    TreeNode r3(3), r2(2, nullptr, &r3), r1(1, nullptr, &r2);
    test("right-chain", {1, 2, 3}, {1, 2, 3}, &r1);
    TreeNode single(8);
    test("single", {8}, {8}, &single);
    test("empty", {}, {}, nullptr);
}
