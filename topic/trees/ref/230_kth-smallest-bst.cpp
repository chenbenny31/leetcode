// LC 230 Kth Smallest Element in a BST: return the k-th smallest value (1-indexed) in a
// binary search tree.
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

// Iterative inorder (ascending order for a BST); stop at the k-th node visited.
// T: O(h + k), S: O(h).
int kthSmallest(TreeNode* root, int k) {
    vector<TreeNode*> stk;   // ancestors whose own value and right subtree are pending
    TreeNode* cur = root;
    while (cur != nullptr || !stk.empty()) {
        while (cur != nullptr) {
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
    TreeNode n1(1), n3(3), n2(2, &n1, &n3), n6(6), n4(4, &n2, &n6);
    test("smallest", &n4, 1, 1);
    test("middle", &n4, 3, 3);
    test("largest", &n4, 5, 6);
    TreeNode single(9);
    test("single", &single, 1, 9);
}
