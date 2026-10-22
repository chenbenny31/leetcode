// LC 173 Binary Search Tree Iterator: iterate a BST in ascending order. next() returns
// the next smallest value, hasNext() says whether one remains. Aim for O(1) average
// time per call and O(h) memory.
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

// Lazy inorder with an explicit stack. next/hasNext: amortized T: O(1), S: O(h).
class BSTIterator {
    // unvisited left spine, top = the next smallest value
    vector<TreeNode*> stk;

    void pushLeft(TreeNode* node) {
        for (; node != nullptr; node = node->left) {
            stk.push_back(node);
        }
    }

public:
    BSTIterator(TreeNode* root) {
        pushLeft(root);
    }

    int next() {
        TreeNode* node = stk.back();
        stk.pop_back();
        pushLeft(node->right);
        return node->val;
    }

    bool hasNext() {
        return !stk.empty();
    }
};

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode n1(1), n3(3), n2(2, &n1, &n3), n6(6), n4(4, &n2, &n6);
    BSTIterator it(&n4);
    vector<int> got;
    while (it.hasNext()) {
        got.push_back(it.next());
    }
    check(got == vector<int>({1, 2, 3, 4, 6}), "ascending");
    check(!it.hasNext(), "exhausted");
    BSTIterator empty(nullptr);
    check(!empty.hasNext(), "empty");
}
