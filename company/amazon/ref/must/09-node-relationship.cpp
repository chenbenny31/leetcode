// Given a binary tree with distinct values and two distinct values u and v in it, return
// "siblings" (same parent), "cousins" (same depth, different parents), or "others".

#include <iostream>
#include <string>
#include <queue>
#include <utility>

using std::cout;
using std::string;
using std::queue;
using std::pair;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
};

// BFS one level at a time, remembering the parent node and depth of u and v.
// T: O(n), S: O(width).
string relationship(TreeNode* root, int u, int v) {
    // parent node and depth of u and v (root's parent is nullptr)
    TreeNode* parentU = nullptr;
    TreeNode* parentV = nullptr;
    int depthU = -1;
    int depthV = -1;
    // (node, its parent), one depth at a time
    queue<pair<TreeNode*, TreeNode*>> q;
    q.push({root, nullptr});
    for (int depth = 0; !q.empty(); depth++) {
        int size = q.size();
        for (int i = 0; i < size; i++) {
            auto [cur, parent] = q.front();
            q.pop();
            if (cur->val == u) {
                parentU = parent;
                depthU = depth;
            }
            if (cur->val == v) {
                parentV = parent;
                depthV = depth;
            }
            if (cur->left != nullptr) { q.push({cur->left, cur}); }
            if (cur->right != nullptr) { q.push({cur->right, cur}); }
        }
    }
    // different depths, or one of them is the root (no parent): neither relation
    if (depthU != depthV || parentU == nullptr) { return "others"; }
    if (parentU == parentV) { return "siblings"; }
    return "cousins";
}

void test(const string& label, TreeNode* root, int u, int v, const string& want) {
    cout << label << (relationship(root, u, v) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    //       1
    //     2   3
    //    4   5  6
    TreeNode n4{4, nullptr, nullptr}, n5{5, nullptr, nullptr}, n6{6, nullptr, nullptr};
    TreeNode n2{2, &n4, nullptr}, n3{3, &n5, &n6};
    TreeNode n1{1, &n2, &n3};
    test("siblings", &n1, 5, 6, "siblings");
    test("cousins", &n1, 4, 6, "cousins");
    test("ancestor", &n1, 2, 4, "others");
    test("diff-depth", &n1, 2, 6, "others");
}
