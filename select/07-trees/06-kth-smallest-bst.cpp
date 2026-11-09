// LC 230 Kth Smallest Element in a BST (Medium). Pattern: iterative inorder, stop early.
// Return the k-th smallest value (1-indexed) in a binary search tree.

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

// Inorder visits a BST in ascending order; return the k-th visited. T: O(h + k), S: O(h).
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
    // 4(2(1, 3), 6)
    TreeNode n1(1);
    TreeNode n3(3);
    TreeNode n2(2, &n1, &n3);
    TreeNode n6(6);
    TreeNode n4(4, &n2, &n6);
    test("smallest", &n4, 1, 1);
    test("middle", &n4, 3, 3);
    test("largest", &n4, 5, 6);
}

// follow-up: frequent inserts / deletes -> keep subtree size in each node; go left if
//   k <= leftSize, stop if k == leftSize + 1, else go right with k - leftSize - 1, O(h)
