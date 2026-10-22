// LC 235 Lowest Common Ancestor of a Binary Search Tree: given a BST and two of its
// nodes p and q, return their lowest common ancestor (a node may be its own ancestor).
#include <iostream>
#include <string>
using std::cout;
using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int v = 0, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(v), left(l), right(r) {}
};

// Walk down from the root; the first node whose value splits p and q (or equals one of
// them) is the LCA. T: O(h), S: O(1).
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    TreeNode* cur = root;
    while (cur != nullptr) {
        if (p->val < cur->val && q->val < cur->val) {
            cur = cur->left;
        } else if (p->val > cur->val && q->val > cur->val) {
            cur = cur->right;
        } else {
            return cur;
        }
    }
    return nullptr;
}

void test(const string& label, TreeNode* root, TreeNode* p, TreeNode* q, TreeNode* want) {
    cout << label << (lowestCommonAncestor(root, p, q) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n0(0), n3(3), n5(5), n7(7), n9(9);
    TreeNode n4(4, &n3, &n5), n2(2, &n0, &n4), n8(8, &n7, &n9), n6(6, &n2, &n8);
    test("split-at-root", &n6, &n2, &n8, &n6);
    test("p-is-ancestor", &n6, &n2, &n4, &n2);
    test("deep-pair", &n6, &n3, &n5, &n4);
}
