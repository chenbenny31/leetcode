// LC 2196: descriptions[i] = [parent, child, isLeft] describe a binary tree with unique
// values, in any order. Build the tree and return its root.

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;
using std::unordered_set;
using std::to_string;

struct TreeNode {
    int val;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;
    TreeNode(int v) : val(v) {}
};

// A hash map from value to node finds or creates each node in O(1) (searching the nodes
// built so far would make it O(n^2)), so the order of descriptions doesn't matter. The
// root is the only parent that is never a child. T: O(n), S: O(n).
TreeNode* createBinaryTree(const vector<vector<int>>& descriptions) {
    // node[v] = the node holding value v
    unordered_map<int, TreeNode*> node;
    unordered_set<int> children;
    for (const auto& d : descriptions) {
        int p = d[0], c = d[1];
        if (!node.count(p)) { node[p] = new TreeNode(p); }
        if (!node.count(c)) { node[c] = new TreeNode(c); }
        if (d[2] == 1) {
            node[p]->left = node[c];
        } else {
            node[p]->right = node[c];
        }
        children.insert(c);
    }
    for (const auto& d : descriptions) {
        if (!children.count(d[0])) { return node[d[0]]; }
    }
    return nullptr;
}

// preorder with '#' for empty children, e.g. "1 2 # # #"
string serialize(TreeNode* root) {
    if (!root) { return "#"; }
    string left = serialize(root->left), right = serialize(root->right);
    return to_string(root->val) + " " + left + " " + right;
}

void destroy(TreeNode* root) {
    if (!root) { return; }
    destroy(root->left);
    destroy(root->right);
    delete root;
}

void test(const string& label, const vector<vector<int>>& descriptions,
          const string& want) {
    TreeNode* root = createBinaryTree(descriptions);
    bool ok = serialize(root) == want;
    destroy(root);
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("two-kids", {{1, 2, 1}, {1, 3, 0}}, "1 2 # # 3 # #");
    test("child-first", {{2, 4, 1}, {1, 2, 1}}, "1 2 4 # # # #");
    test("right-only", {{5, 6, 0}}, "5 # 6 # #");
}
