// LC 236 Lowest Common Ancestor of a Binary Tree (Medium). Pattern: post-order return.
// Given two nodes p and q of a binary tree, return their lowest common ancestor
// (a node may be its own ancestor).

#include <iostream>
#include <string>

using std::cout;
using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x, TreeNode* l = nullptr, TreeNode* r = nullptr)
        : val(x), left(l), right(r) {}
};

// LC 1650 node: only the parent link is needed to find the LCA
struct ParentNode {
    int val;
    ParentNode* parent;
    ParentNode(int x, ParentNode* p = nullptr) : val(x), parent(p) {}
};

// Each call returns p or q if its subtree holds one, else null; the first node that
// gets hits from both sides is the LCA. T: O(n), S: O(h).
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    // reaching p or q stops the descent: the other one below it makes it the LCA anyway
    if (!root || root == p || root == q) { return root; }
    TreeNode* left = lowestCommonAncestor(root->left, p, q);
    TreeNode* right = lowestCommonAncestor(root->right, p, q);
    if (left && right) { return root; }
    return left ? left : right;
}

// LC 235 BST: walk down from the root; both smaller go left, both larger go right,
// else p and q split here (or one of them is here). T: O(h), S: O(1).
TreeNode* lcaBST(TreeNode* root, TreeNode* p, TreeNode* q) {
    TreeNode* cur = root;
    while (cur) {
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

// LC 1650: like list intersection, walk up from p and q; a pointer that passes the root
// restarts at the other node. Both travel depth(p) + depth(q) and meet at the LCA.
// T: O(h), S: O(1).
ParentNode* lcaWithParent(ParentNode* p, ParentNode* q) {
    ParentNode* fromP = p;
    ParentNode* fromQ = q;
    while (fromP != fromQ) {
        fromP = fromP ? fromP->parent : q;
        fromQ = fromQ ? fromQ->parent : p;
    }
    return fromP;
}

void test(const string& label, TreeNode* root, TreeNode* p, TreeNode* q, TreeNode* want) {
    cout << label << (lowestCommonAncestor(root, p, q) == want ? " PASS\n" : " FAIL\n");
}

void testBST(const string& label, TreeNode* root, TreeNode* p, TreeNode* q,
             TreeNode* want) {
    bool ok = lcaBST(root, p, q) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

void testParent(const string& label, ParentNode* p, ParentNode* q, ParentNode* want) {
    bool ok = lcaWithParent(p, q) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // 1(2(4, 5), 3(6, 7))
    TreeNode n4(4);
    TreeNode n5(5);
    TreeNode n6(6);
    TreeNode n7(7);
    TreeNode n2(2, &n4, &n5);
    TreeNode n3(3, &n6, &n7);
    TreeNode n1(1, &n2, &n3);
    test("split-at-root", &n1, &n4, &n7, &n1);
    test("p-is-ancestor", &n1, &n2, &n5, &n2);
    test("sibling-leaves", &n1, &n6, &n7, &n3);
    // BST 6(2(1, 4), 8(7, 9))
    TreeNode b1(1);
    TreeNode b4(4);
    TreeNode b7(7);
    TreeNode b9(9);
    TreeNode b2(2, &b1, &b4);
    TreeNode b8(8, &b7, &b9);
    TreeNode b6(6, &b2, &b8);
    testBST("bst-split-at-root", &b6, &b1, &b9, &b6);
    testBST("bst-both-left", &b6, &b1, &b4, &b2);
    testBST("bst-p-is-ancestor", &b6, &b8, &b7, &b8);
    // 1(2(4, 5), 3) with parent links
    ParentNode r1(1);
    ParentNode r2(2, &r1);
    ParentNode r3(3, &r1);
    ParentNode r4(4, &r2);
    ParentNode r5(5, &r2);
    testParent("parent-diff-depth", &r4, &r3, &r1);
    testParent("parent-is-ancestor", &r2, &r5, &r2);
    testParent("parent-siblings", &r4, &r5, &r2);
}
