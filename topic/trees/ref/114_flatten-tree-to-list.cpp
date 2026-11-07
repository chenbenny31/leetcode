// LC 114 Flatten Binary Tree to Linked List: relink the tree in place into a preorder
// chain along right pointers.
// Follow-up: O(1) extra space.
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

// Appends the subtree's nodes in preorder. T: O(n), S: O(h).
void preorder(TreeNode* node, vector<TreeNode*>& order) {
    if (node == nullptr) { return; }
    order.push_back(node);
    preorder(node->left, order);
    preorder(node->right, order);
}

// Collect the nodes in preorder, then link each to the next. T: O(n), S: O(n).
void flatten(TreeNode* root) {
    vector<TreeNode*> order;
    preorder(root, order);

    int n = order.size();
    for (int i = 0; i < n; i++) {
        order[i]->left = nullptr;
        order[i]->right = i + 1 < n ? order[i + 1] : nullptr;
    }
}

// Morris-style: hang the right subtree off the left subtree's rightmost node, then move
// the left subtree to the right and step down. Each rightmost path is walked once.
// T: O(n), S: O(1).
void flattenInPlace(TreeNode* root) {
    TreeNode* cur = root;
    while (cur != nullptr) {
        if (cur->left != nullptr) {
            TreeNode* tail = cur->left;   // rightmost node of the left subtree
            while (tail->right != nullptr) { tail = tail->right; }
            tail->right = cur->right;
            cur->right = cur->left;
            cur->left = nullptr;
        }

        cur = cur->right;
    }
}

// true if root is a right-only chain with values want, in order. T: O(n), S: O(1).
bool isChain(TreeNode* root, const vector<int>& want) {
    for (int v : want) {
        if (root == nullptr || root->left != nullptr || root->val != v) { return false; }
        root = root->right;
    }

    return root == nullptr;
}

void test(const string& label, TreeNode* root, const vector<int>& want) {
    flatten(root);
    cout << label << (isChain(root, want) ? " PASS\n" : " FAIL\n");
}

int main() {
    TreeNode a3(3), a2(2, &a3), a5(5), a4(4, &a5), a1(1, &a2, &a4);
    test("both-sides", &a1, {1, 2, 3, 4, 5});

    TreeNode c3(3), c2(2, &c3), c1(1, &c2);
    test("left-chain", &c1, {1, 2, 3});

    test("empty", nullptr, {});
}
