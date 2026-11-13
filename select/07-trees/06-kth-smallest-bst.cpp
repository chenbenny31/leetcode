// LC 230 Kth Smallest Element in a BST (Medium) — iterative inorder, stop early
// Return the k-th smallest value (1-indexed) in a binary search tree.
// Clarify: 1 <= k <= node count; duplicate values; tree changed between queries; tree
//   height (balanced or skewed).

#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(x), left(l), right(r) {}
};

// Core idea: inorder visits a BST in ascending order, so walk it with an explicit stack
// and return the k-th node visited. T: O(h + k), S: O(h).
int kthSmallest(TreeNode* root, int k) {
    // ancestors whose own value and right subtree are still pending
    vector<TreeNode*> stk;
    TreeNode* cur = root;
    while (cur || !stk.empty()) {
        while (cur) {
            stk.push_back(cur);
            cur = cur->left;
        }
        cur = stk.back();
        stk.pop_back();
        k--;
        if (k == 0) { return cur->val; }
        cur = cur->right;
    }
    return -1;
}

void test(const string& label, TreeNode* root, int k, int want) {
    cout << label << (kthSmallest(root, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n1(1);
    TreeNode n3(3);
    TreeNode n2(2, &n1, &n3);
    TreeNode n6(6);
    TreeNode n4(4, &n2, &n6);
    test("smallest", &n4, 1, 1);
    test("middle", &n4, 3, 3);
    test("largest", &n4, 5, 6);
    return 0;
}

// follow-up (ideas only):
// - frequent inserts / deletes -> keep the subtree size in each node; go left if
//   k <= leftSize, stop if k == leftSize + 1, else go right with k - leftSize - 1, O(h)
// - k-th largest -> mirrored inorder: push the right spine, then step to the left child
// - BST iterator (LC 173) -> the same stack: next() pops a node, then pushes the left
//   spine of its right child
// - inorder successor of a node (LC 285) -> walk down from the root, remembering the last
//   node where the walk went left
