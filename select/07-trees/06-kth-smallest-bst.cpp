// LC 230 Kth Smallest Element in a BST (Medium) — iterative inorder, stop early
// return the k-th smallest value (1-indexed) in a BST
//
// clarify:
// k range -> 1 <= k <= node cnt
// dup values -> count each
// tree changes between queries -> follow-up (subtree size)
// height -> balanced O(log n), skewed O(n)

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

// core idea: BST inorder is ascending -> walk w/ explicit stack, return k-th visited
// T: O(h + k), S: O(h)
int kthSmallest(TreeNode* root, int k) {
    // ancestors whose own value + right subtree are pending
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

// follow-up:
// frequent inserts / deletes -> subtree size per node; compare k w/ leftSize, O(h)
// k-th largest -> mirrored inorder: push right spine, then step to left child
// BST iterator (LC 173) -> same stack: next() pops, then pushes left spine of right child
// inorder successor (LC 285) -> walk down from root, remember last node where went left
