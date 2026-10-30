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

// BFS records (parent value, depth) for u and v in one pass. T: O(n), S: O(n).
string relationship(TreeNode* root, int u, int v) {
    int parentU = -1, parentV = -1, depthU = -1, depthV = -1;
    // queue of (node, parent value), processed one depth at a time
    queue<pair<TreeNode*, int>> q;
    q.push({root, -1});
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
            if (cur->left) { q.push({cur->left, cur->val}); }
            if (cur->right) { q.push({cur->right, cur->val}); }
        }
    }
    if (depthU != depthV || depthU == 0) { return "others"; }
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
