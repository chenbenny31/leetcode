// LC 337 House Robber III: houses form a binary tree. Two directly linked houses cannot
// both be robbed. Return the maximum cash.
#include <iostream>
#include <string>
#include <utility>
#include <algorithm>
using std::cout;
using std::max;
using std::pair;
using std::string;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

// Tree DP returning {best if node is robbed, best if node is skipped}.
// T: O(n), S: O(h).
pair<int, int> dfs(TreeNode* node) {
    if (node == nullptr) { return {0, 0}; }
    auto [lTake, lSkip] = dfs(node->left);
    auto [rTake, rSkip] = dfs(node->right);
    // robbing node forces both children to be skipped
    int take = node->val + lSkip + rSkip;
    int skip = max(lTake, lSkip) + max(rTake, rSkip);
    return {take, skip};
}

int rob(TreeNode* root) {
    auto [take, skip] = dfs(root);
    return max(take, skip);
}

void test(const string& label, TreeNode* root, int want) {
    if (rob(root) == want) {
        cout << label << " PASS\n";
    } else {
        cout << label << " FAIL\n";
    }
}

int main() {
    TreeNode a(3), b(4), c(1), d(3);
    a.left = &b;
    b.left = &c;
    b.right = &d;
    test("grandchildren", &a, 7);
    TreeNode r(1), s(4), t(5);
    r.left = &s;
    r.right = &t;
    test("skip-root", &r, 9);
    TreeNode one(5);
    test("single", &one, 5);
    test("null", nullptr, 0);
}
