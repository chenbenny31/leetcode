// LC 2641 Cousins in Binary Tree II: replace every node's value with the sum of its
// cousins (same depth, different parent). Return the root.

#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

struct TreeNode {
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode(int v) : val(v) {}
};

// Level by level: a child's new value = whole next level's sum - its siblings' sum
// (siblings = both children of the same parent). T: O(n), S: O(width).
TreeNode* replaceValueInTree(TreeNode* root) {
    root->val = 0;
    vector<TreeNode*> level{root};
    while (!level.empty()) {
        // sum of every value on the next level
        long long nextSum = 0;
        for (TreeNode* cur : level) {
            if (cur->left != nullptr) { nextSum += cur->left->val; }
            if (cur->right != nullptr) { nextSum += cur->right->val; }
        }
        vector<TreeNode*> nextLevel;
        for (TreeNode* cur : level) {
            // read both children before overwriting either
            long long siblings = 0;
            if (cur->left != nullptr) { siblings += cur->left->val; }
            if (cur->right != nullptr) { siblings += cur->right->val; }
            if (cur->left != nullptr) {
                cur->left->val = nextSum - siblings;
                nextLevel.push_back(cur->left);
            }
            if (cur->right != nullptr) {
                cur->right->val = nextSum - siblings;
                nextLevel.push_back(cur->right);
            }
        }
        level = nextLevel;
    }
    return root;
}

void check(bool ok, const string& label) {
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    // 5 has children 4, 9; 4 has children 1, 10; 9 has right child 7
    TreeNode a(5), b(4), c(9), d(1), e(10), f(7);
    a.left = &b;
    a.right = &c;
    b.left = &d;
    b.right = &e;
    c.right = &f;
    replaceValueInTree(&a);
    check(a.val == 0, "root-zero");
    check(b.val == 0 && c.val == 0, "only-siblings");
    check(d.val == 7 && e.val == 7 && f.val == 11, "cousins-sum");
}
