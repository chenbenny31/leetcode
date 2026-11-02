// uniq vals? yes
// u, v both in tree: yes, u!=v
// def -> siblings=same-parent, cousins=same-depth+diff-parent, others
// vals be neg? yes
// binary or n-ary
// return tyep?

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

// BFS one level at a time, keep parent node and depth of u/v, T: O(n), S: O(width)
string relationship(TreeNode* root, int u, int v) {
    TreeNode* parentU = nullptr;
    TreeNode* parentV = nullptr;
    int depthU = -1;
    int depthV = -1;
    queue<pair<TreeNode*, TreeNode*>> q; // (node, its-parent)
    q.push({root, nullptr});
    for (int depth = 0; !q.empty(); depth++) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            auto [cur, parent] = q.front(); // must be copy, NO &
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
