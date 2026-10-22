// LC 108 Convert Sorted Array to Binary Search Tree: convert an ascending array into a
// height-balanced BST (subtree heights differ by at most 1).
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::max;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// Builds the subtree from nums[lo, hi): the middle element is the root, so both sides
// differ in size by at most 1.
TreeNode* build(const vector<int>& nums, int lo, int hi) {
    if (lo >= hi) { return nullptr; }
    int mid = lo + (hi - lo) / 2;
    return new TreeNode(nums[mid], build(nums, lo, mid), build(nums, mid + 1, hi));
}

// Middle element as root, recurse on both halves. T: O(n), S: O(log n) stack.
TreeNode* sortedArrayToBST(vector<int>& nums) {
    int n = nums.size();
    return build(nums, 0, n);
}

void inorder(TreeNode* root, vector<int>& out) {
    if (root == nullptr) { return; }
    inorder(root->left, out);
    out.push_back(root->val);
    inorder(root->right, out);
}

// height of a subtree, or -1 if some node has subtrees whose heights differ by over 1
int balancedHeight(TreeNode* root) {
    if (root == nullptr) { return 0; }
    int left = balancedHeight(root->left);
    int right = balancedHeight(root->right);
    if (left < 0 || right < 0 || left - right > 1 || right - left > 1) { return -1; }
    return 1 + max(left, right);
}

void freeTree(TreeNode* root) {
    if (root == nullptr) { return; }
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

void test(const string& label, vector<int> nums) {
    TreeNode* root = sortedArrayToBST(nums);
    vector<int> got;
    inorder(root, got);
    bool ok = got == nums && balancedHeight(root) >= 0;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
    freeTree(root);
}

int main() {
    test("odd", {-5, -1, 0, 3, 8});
    test("even", {1, 2, 3, 4});
    test("single", {7});
    test("empty", {});
}
